#ifndef SCHOOL_ASCII
#define SCHOOL_ASCII

namespace school
{

    void historyOfASCIIAndTheInternet()
    {
        // clang-format off

        // 1865 the International Telegraph Union (ITU) starts standardizing telegraph codes.
        // But if you were doing things differently, then you had a reason, which is usually more important than following someone else's standard.
        // So years pass, and a dozen different variations of these standards are used.
        // (but Western Union owns everything.  So...)
        // 1924 the ITU authors new 5bit standard called ITA-2 (International Telegraph Alphabet - Version 2)
        // The ITU based ITA-2 on Western Union's codes, thinking that would get everybody on board.
        // " But if you were doing things differently, then you had a reason, which is usually more important than following someone else's standard.
        // " So years pass, and a dozen different variations of the ITA-2 standard are used.
        // https://xkcd.com/927/
        //
        // But then Teleprinters were invented.
        // Moving precisely worded legal contracts or financial spreadsheets via phone conversations doesn’t work.
        // So Teleprinters made big business more money and big military more powerful, and they quickly spread world wide.
        // Almost all Teleprinters were based on the ITA-2.
        // Now the whole world had a very good reason to abandon their own code set and adopt an international standard.
        // 
        // A patchwork of cables and repurposed phone lines spread everywhere to connect Teleprinters, called Telex.
        // (Note that this was NOT a network like the internet...)
        // Somebody figured out how to send the 5bit ITA-2 codes over radio (just like the Telegraph).
        // But radio channels are full of noise and it didn't work well, so they invented special codes for STX/ETX.
        // This is modulation/de-modulation, some of the first modems.
        // Telex jumped the wire and reached across the oceans, becoming truly global.
        // Somebody figured out that making Telex work over noisy radio channels is the same as making it work over noisy phone lines.
        // Telex didn't need custom or dedicated cables or radio anymore -just a phone call.
        // This made Teleprinting cheap, and it spread even farther.
        // Then it got faster and faster, until the speed was limited by the mechanical gears in the Teleprinters.
        //
        // As Telex evolved, it needed codes for things that were not letters.  (STX/ETX/LF/CR/BEL/etc.)
        //  10 LF "Line Feed" -moved the print position down one line
        //  13 CR "Carrige Recturn" -moved the print position down one line and all the way to the left
        //   7 BEL "Bell" -notified attendats that some transmission was complete
        // Bell Labs was the Google god of the 1960's, so it set out to create a new standard that extended the ITA-2.
        // The people at Bell Labs were smart, knew their history, and figured out how to make standards that the world actually used.
        // They created a comity to draft a standard that included representatives from other businesses and researchers, and it worked.
        // 1963 Bell Labs publishes the 7bit American Standard Code for Information Interchange (ASCII).
        // Everybody used it.
        // This new way of using well represented comities to make standards is widely used today.
        // C++ is a language standard that evolves over time in this same fashion.
        // ASCII evolved over time as well, now 8bits, etc.
        // ASCII became the foundation for programing languages, C, C++, Java, Python, HTML, and MANY others...
        // Today we use Unicode more and more, even though it is kind of a mess...
        //
        // So the world was a giant Telex network that allowed the millitary to coordinate in real time, and become FAR more powerful.
        // But any serious war would be nuclear, and one nuclear strike could take out phone relay stations.
        // That would eliminate Telex, and THAT would render the millitary essentially helpless.
        // The US Millitary solved this problem by designing a distributed network of nodes.
        // This was a breakthrough:  Every node is the same, and it is the nodes that move the data.
        // This network can handle lots of nodes going up and down all the time.
        // This was the network that replaced Telex and became the internet.

        // clang-format on
    }

} // namespace school
#endif // SCHOOL_ASCII
