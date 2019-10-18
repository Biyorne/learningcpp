// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// main.cpp
//
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

#include <SFML/Graphics.hpp>

//

constexpr std::size_t operator"" _st(unsigned long long number)
{
    return static_cast<std::size_t>(number);
}

//

// size and position

template <typename T>
void scale(T & thing, const sf::Vector2f & size)
{
    // skip if source size is zero (or close) to avoid dividing by zero below
    const auto localBounds(thing.getLocalBounds());
    if ((localBounds.width < 1.0f) || (localBounds.height < 1.0f))
    {
        return;
    }

    float scale(1.0f);
    if ((size.x - localBounds.width) < (size.y - localBounds.height))
    {
        scale = (size.x / localBounds.width);
    }
    else
    {
        scale = (size.y / localBounds.height);
    }

    thing.setScale(scale, scale);
    thing.setOrigin(localBounds.left, localBounds.top);
}

template <typename T>
void scale(T & thing, const sf::FloatRect & rect)
{
    scale(thing, { rect.width, rect.height });
}

template <typename T>
void center(T & thing, const sf::FloatRect & rect)
{
    const sf::Vector2f size(thing.getGlobalBounds().width, thing.getGlobalBounds().height);
    const sf::Vector2f rectPos(rect.left, rect.top);
    const sf::Vector2f rectSize(rect.width, rect.height);
    const sf::Vector2f rectPosCenter(rectPos + (rectSize * 0.5f));
    thing.setPosition(rectPosCenter - (size * 0.5f));
}

template <typename T>
void centerAndScale(T & thing, const sf::FloatRect & rect)
{
    scale(thing, rect);
    center(thing, rect);
}

//

class FullPassFragmentShader
{
protected:
    FullPassFragmentShader(const std::string & fragmentShader)
        : m_shader()
        , m_verts(sf::TrianglesStrip, 4)
        , m_renderStates()
    {
        m_shader.loadFromMemory(m_fullPassVertexShaderCode, fragmentShader);

        // sf::VertexArray zero initializes everything so only setting the non-zeros here
        m_verts[0].texCoords.y = 1.0f;
        m_verts[1].texCoords = { 1.0f, 1.0f };
        m_verts[3].texCoords.x = 1.0f;

        m_renderStates.shader = &m_shader;
        m_renderStates.blendMode = sf::BlendNone;
    }

    template <typename... Args_t>
    void setUniform(const std::string & name, const Args_t &... args)
    {
        m_shader.setUniform(name, args...);
    }

    void draw(sf::RenderTarget & target) const
    {
        // only setting what changes here
        m_verts[3].position = sf::Vector2f(target.getSize());
        m_verts[1].position.x = m_verts[3].position.x;
        m_verts[2].position.y = m_verts[3].position.y;

        target.draw(m_verts, m_renderStates);
    }

private:
    sf::Shader m_shader;
    mutable sf::VertexArray m_verts;
    sf::RenderStates m_renderStates;

    static inline const std::string m_fullPassVertexShaderCode { "\
void main()\
{\
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\
    gl_TexCoord[0] = gl_MultiTexCoord0;\
}" };
};

//

struct BrightFilterShader : public FullPassFragmentShader
{
    BrightFilterShader()
        : FullPassFragmentShader(m_fragmentShaderCode)
    {}

    void draw(const sf::RenderTexture & input, sf::RenderTexture & output)
    {
        setUniform("source", input.getTexture());
        FullPassFragmentShader::draw(output);
        output.display();
    }

    static inline const std::string m_fragmentShaderCode { "\
uniform sampler2D source;\
\
const float Threshold = 0.7;\
const float Factor = 4.0;\
\
void main()\
{\
    vec4 sourceFragment = texture2D(source, gl_TexCoord[0].xy);\
    float luminance = sourceFragment.r * 0.2126 + sourceFragment.g * 0.7152 + sourceFragment.b *0.0722;\
    sourceFragment *= clamp(luminance - Threshold, 0.0, 1.0) * Factor;\
    gl_FragColor = sourceFragment;\
}" };
};

//

struct AddShader : public FullPassFragmentShader
{
    AddShader()
        : FullPassFragmentShader(m_fragmentShaderCode)
    {}

    void draw(
        const sf::RenderTexture & source,
        const sf::RenderTexture & bloom,
        sf::RenderTarget & output)
    {
        setUniform("source", source.getTexture());
        setUniform("bloom", bloom.getTexture());
        FullPassFragmentShader::draw(output);
    }

    static inline const std::string m_fragmentShaderCode { "\
uniform sampler2D source;\
uniform sampler2D bloom;\
\
void main()\
{\
    vec4 sourceFragment = texture2D(source, gl_TexCoord[0].xy);\
    vec4 bloomFragment = texture2D(bloom, gl_TexCoord[0].xy);\
    gl_FragColor = sourceFragment + bloomFragment;\
}" };
};

//

struct DownSampleShader : public FullPassFragmentShader
{
    DownSampleShader()
        : FullPassFragmentShader(m_fragmentShaderCode)
    {}

    void draw(const sf::RenderTexture & input, sf::RenderTexture & output)
    {
        setUniform("source", input.getTexture());
        setUniform("sourceSize", sf::Vector2f(input.getSize()));
        FullPassFragmentShader::draw(output);
        output.display();
    }

    static inline const std::string m_fragmentShaderCode { "\
uniform sampler2D 	source;\
uniform vec2 		sourceSize;\
\
void main()\
{\
    vec2 pixelSize = vec2(1.0 / sourceSize.x, 1.0 / sourceSize.y);\
    vec2 textureCoordinates = gl_TexCoord[0].xy;\
    vec4 color = texture2D(source, textureCoordinates);\
    color += texture2D(source, textureCoordinates + vec2(1.0,  0.0) * pixelSize);\
    color += texture2D(source, textureCoordinates + vec2(-1.0,  0.0) * pixelSize);\
    color += texture2D(source, textureCoordinates + vec2(0.0,  1.0) * pixelSize);\
    color += texture2D(source, textureCoordinates + vec2(0.0, -1.0) * pixelSize);\
    color += texture2D(source, textureCoordinates + vec2(1.0,  1.0) * pixelSize);\
    color += texture2D(source, textureCoordinates + vec2(-1.0, -1.0) * pixelSize);\
    color += texture2D(source, textureCoordinates + vec2(1.0, -1.0) * pixelSize);\
    color += texture2D(source, textureCoordinates + vec2(-1.0,  1.0) * pixelSize);\
    gl_FragColor = color / 9.0;\
}" };
};

//

struct BlurShader : public FullPassFragmentShader
{
    explicit BlurShader(const std::size_t multiPassCount = 2)
        : FullPassFragmentShader(m_fragmentShaderCode)
        , m_multiPassCount(multiPassCount)
    {}

    std::size_t multiPassCount() const { return m_multiPassCount; }
    void multiPassCount(const std::size_t count) { m_multiPassCount = count; }

    void draw(
        const sf::RenderTexture & input,
        sf::RenderTexture & output,
        const sf::Vector2f & offsetFactor)
    {
        setUniform("source", input.getTexture());
        setUniform("offsetFactor", offsetFactor);
        FullPassFragmentShader::draw(output);
        output.display();
    }

    void drawMultiPass(sf::RenderTexture & left, sf::RenderTexture & right)
    {
        const sf::Vector2f size(left.getSize());

        sf::Vector2f offsetFactorVert(0.0f, (1.0f / size.y));
        sf::Vector2f offsetFactorHoriz((1.0f / size.x), 0.0f);

        const float blurMultiplier(1.25f);
        for (std::size_t count(0); count < m_multiPassCount; ++count)
        {
            draw(left, right, offsetFactorVert);
            draw(right, left, offsetFactorHoriz);

            offsetFactorVert *= blurMultiplier;
            offsetFactorHoriz *= blurMultiplier;
        }
    }

private:
    std::size_t m_multiPassCount;

    // this is actually a Gaussian Blur
    static inline const std::string m_fragmentShaderCode { "\
uniform sampler2D 	source;\
uniform vec2 		offsetFactor;\
\
void main()\
{\
    vec2 textureCoordinates = gl_TexCoord[0].xy;\
    vec4 color = vec4(0.0);\
    color += texture2D(source, textureCoordinates - 4.0 * offsetFactor) * 0.0162162162;\
    color += texture2D(source, textureCoordinates - 3.0 * offsetFactor) * 0.0540540541;\
    color += texture2D(source, textureCoordinates - 2.0 * offsetFactor) * 0.1216216216;\
    color += texture2D(source, textureCoordinates - offsetFactor) * 0.1945945946;\
    color += texture2D(source, textureCoordinates) * 0.2270270270;\
    color += texture2D(source, textureCoordinates + offsetFactor) * 0.1945945946;\
    color += texture2D(source, textureCoordinates + 2.0 * offsetFactor) * 0.1216216216;\
    color += texture2D(source, textureCoordinates + 3.0 * offsetFactor) * 0.0540540541;\
    color += texture2D(source, textureCoordinates + 4.0 * offsetFactor) * 0.0162162162;\
    gl_FragColor = color;\
}" };
};

//

class BloomEffect
{
    typedef std::array<sf::RenderTexture, 2> RenderTextureArray;

public:
    BloomEffect()
        : m_addShader()
        , m_blurShader()
        , m_downSampleShader()
        , m_brightFilterShader()
        , m_brightnessTexture()
        , m_firstPassTextures()
        , m_secondPassTextures()
    {}

    // don't forget to call output.display() after this function!
    void apply(const sf::RenderTexture & input, sf::RenderTarget & output)
    {
        // calling this every time could thrash sf::RenderTextures.  You have been warned.
        resizeTextures(input.getSize());

        m_brightFilterShader.draw(input, m_brightnessTexture);

        m_downSampleShader.draw(m_brightnessTexture, m_firstPassTextures[0]);
        m_blurShader.drawMultiPass(m_firstPassTextures[0], m_firstPassTextures[1]);

        m_downSampleShader.draw(m_firstPassTextures[0], m_secondPassTextures[0]);
        m_blurShader.drawMultiPass(m_secondPassTextures[0], m_secondPassTextures[1]);

        m_addShader.draw(m_firstPassTextures[0], m_secondPassTextures[0], m_firstPassTextures[1]);
        m_firstPassTextures[1].display();

        m_addShader.draw(input, m_firstPassTextures[1], output);
    }

    std::size_t blurMultiPassCount() const { return m_blurShader.multiPassCount(); }
    void blurMultiPassCount(const std::size_t count) { m_blurShader.multiPassCount(count); }

private:
    void resizeTextures(const sf::Vector2u & inputSize)
    {
        if (m_brightnessTexture.getSize() == inputSize)
        {
            return;
        }

        m_brightnessTexture.create(inputSize.x, inputSize.y);
        m_brightnessTexture.setSmooth(true);

        m_firstPassTextures[0].create(inputSize.x / 2, inputSize.y / 2);
        m_firstPassTextures[0].setSmooth(true);

        m_firstPassTextures[1].create(inputSize.x / 2, inputSize.y / 2);
        m_firstPassTextures[1].setSmooth(true);

        m_secondPassTextures[0].create(inputSize.x / 4, inputSize.y / 4);
        m_secondPassTextures[0].setSmooth(true);

        m_secondPassTextures[1].create(inputSize.x / 4, inputSize.y / 4);
        m_secondPassTextures[1].setSmooth(true);
    }

private:
    AddShader m_addShader;
    BlurShader m_blurShader;
    DownSampleShader m_downSampleShader;
    BrightFilterShader m_brightFilterShader;

    sf::RenderTexture m_brightnessTexture;
    RenderTextureArray m_firstPassTextures;
    RenderTextureArray m_secondPassTextures;
};

//

int main()
{
    std::cout.imbue(std::locale("")); // this is only to put commas in big numbers

    if (!sf::Shader::isAvailable())
    {
        std::cout << "Shaders are NOT supported.  Your video card sucks." << std::endl;
        return EXIT_SUCCESS;
    }

    bool willBloom(false);
    bool isFullscreen(false);
    const std::string appName("Shaders");
    const sf::VideoMode videoMode(1280, 1024, sf::VideoMode::getDesktopMode().bitsPerPixel);

    sf::RenderWindow window(videoMode, appName, sf::Style::Default);

    const sf::Vector2f windowSize(window.getSize());
    const sf::FloatRect windowRect({}, windowSize);

    sf::RenderTexture sideTexture;
    sideTexture.create(window.getSize().x, window.getSize().y);
    const sf::Vector2f sideTextureSize(sideTexture.getSize());

    std::size_t index(0);
    std::vector<std::unique_ptr<sf::Texture>> textures;
    std::vector<sf::Sprite> sprites;
    {
        const std::string imageExtensions(".bmp/.jpg/.jpeg/.png/.tga");

        std::filesystem::directory_iterator dirIter(std::filesystem::current_path());

        for (const std::filesystem::directory_entry & entry : dirIter)
        {
            if (!entry.is_regular_file())
            {
                continue;
            }

            const std::string extension(entry.path().extension().string());
            if (extension.empty())
            {
                continue;
            }

            if (imageExtensions.find(extension) >= imageExtensions.size())
            {
                continue;
            }

            auto texture = std::make_unique<sf::Texture>();
            if (texture->loadFromFile(entry.path().string()))
            {
                sprites.emplace_back(*texture);
                textures.push_back(std::move(texture));
            }
        }
    }

    if (sprites.empty())
    {
        std::cout << "No images found to demonstrate the shader effects.  Put some images next to "
                     "the executable and try running again."
                  << std::endl;

        return EXIT_SUCCESS;
    }

    BloomEffect bloomEffect;

    sf::Clock clock;

    while (window.isOpen())
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (sf::Event::Closed == event.type)
            {
                window.close();
                break;
            }

            if (sf::Event::KeyPressed != event.type)
            {
                continue;
            }

            if (sf::Keyboard::Escape == event.key.code)
            {
                window.close();
                break;
            }

            if (sf::Keyboard::B == event.key.code)
            {
                if (!sf::Shader::isAvailable())
                {
                    std::cout << "'B' Keypress ignored because shaders are NOT supported.  Your "
                                 "video card sucks."
                              << std::endl;
                    continue;
                }

                willBloom = !willBloom;
            }
            else if (sf::Keyboard::F == event.key.code)
            {
                isFullscreen = !isFullscreen;

                window.close();

                window.create(
                    videoMode,
                    appName,
                    ((isFullscreen) ? sf::Style::Fullscreen : sf::Style::Default));
            }
            else if (sf::Keyboard::R == event.key.code)
            {
                index = 0;
                bloomEffect.blurMultiPassCount(2);
                willBloom = false;
            }
            else if (sf::Keyboard::Right == event.key.code)
            {
                if (index < (sprites.size() - 1))
                {
                    ++index;
                }
                else
                {
                    index = 0;
                }
            }
            else if (sf::Keyboard::Left == event.key.code)
            {
                if (index > 0)
                {
                    --index;
                }
                else
                {
                    index = (sprites.size() - 1);
                }
            }
            else if (sf::Keyboard::Up == event.key.code)
            {
                bloomEffect.blurMultiPassCount(bloomEffect.blurMultiPassCount() + 1);
            }
            else if (sf::Keyboard::Down == event.key.code)
            {
                const std::size_t multiPassCountOrig(bloomEffect.blurMultiPassCount());

                if (multiPassCountOrig > 0)
                {
                    bloomEffect.blurMultiPassCount(multiPassCountOrig - 1);
                }
            }
        }

        // draw
        {
            window.clear();

            sideTexture.clear();

            if (index < sprites.size())
            {
                auto & sprite = sprites.at(index);
                centerAndScale(sprite, windowRect);
                sideTexture.draw(sprite);
            }

            sideTexture.display();

            if (willBloom)
            {
                bloomEffect.apply(sideTexture, window);
            }
            else
            {
                sf::VertexArray verts(sf::Quads, 4);

                verts[0] = sf::Vertex(sf::Vector2f(0, 0), sf::Vector2f(0, 0));

                verts[1]
                    = sf::Vertex(sf::Vector2f(windowSize.x, 0), sf::Vector2f(sideTextureSize.x, 0));

                verts[2] = sf::Vertex(
                    sf::Vector2f(windowSize.x, windowSize.y),
                    sf::Vector2f(sideTextureSize.x, sideTextureSize.y));

                verts[3]
                    = sf::Vertex(sf::Vector2f(0, windowSize.y), sf::Vector2f(0, sideTextureSize.y));

                sf::RenderStates states;
                states.texture = &sideTexture.getTexture();

                window.draw(verts, states);
            }

            window.display();
        }
    }

    return EXIT_SUCCESS;
}
