# Radioactive decay based random number generator on steroids (STM32G474RE)
An STM32 HAL example of a true random number generator using a Geiger counter.

> [!IMPORTANT]
> The experiment can be carried out in two ways: a totally safe one using a background radiation around us and a doped one using uranium ceramic. Both of them let you generate a stream of truly random bits. The difference lies in the bitrate they offer. In my case the background radiation was insufficient to generate, in a reasonable time frame, a stream of bits long enough for some conclusive statistical testing of its randomness.

> [!CAUTION]
> Dope your Geiger counter with a radioactive sample if and ONLY if you know what you are doing :exclamation: Even if individuals can in you country legally buy samples of uranium ore/ceramic[^1], such materials require proper handling to be considered safe for human health. Educate yourself on the topic before you make any purchasing decisions - and it is not only about its radioactivity :radioactive: but also about its toxicity :vomiting_face:

[^1]: [Sprawdź licznik Geigera czy działa?! URAN spiek](https://allegrolokalnie.pl/oferta/sprawdz-licznik-geigera-czy-dziala-uran-spiek)

![Uranium ore in action](/Assets/Images/geiger_rng_in_action.jpg)

Uranium ceramic sample inside the counter stuck to the tube - hence the over standard readings:

![Uranium ore in action](/Assets/Images/uranium_ceramic_sample.jpg)

The algorithm itself is extremely simple. As the decay events are fundamentally unpredictable it is sufficient to measure time span between four consecutive event detections and compare the length of two time intervals to generate a random bit. In my case the resolution of HAL_GetTick() proved to be sufficient to pass selected tests for randomness collected by NIST in "A Statistical Test Suite for Random and Pseudorandom Number Generators for Cryptographic Applications"[^2]. Why just selected ones? All ones that can be performed on ca. 100 000 bits. Some tests have to be performed on much longer streams to be considered conclusive. My generator operates at the level of 30 random bytes per minute. Generating of 1 000 000 bits would take ca. 3 days. Doable but still not accomplished at the time of writing. And this something definitely in my mind - notice the Winbond W25Q128 in the photo :slightly_smiling_face: Two tests require even more bits than that, e.g. 10 000 000 bits are recommended[^3].

The low bitrate of TRNGs is not something uncommon. They can be used as an entropy source for PRNGs that can offer higher bitrates[^4].

> [!TIP]
> If the resolution of 1 ms is not sufficient in your case, go for a timer in the input capture mode.

[^2]: [Random Bit Generation](https://csrc.nist.gov/projects/random-bit-generation/documentation-and-software) (National Institute of Standards and Technology)

[^3]: [NIST random excursion results](https://crypto.stackexchange.com/questions/71635/nist-random-excursion-results) (StackExchange)

[^4]: [Hardware random number generator](https://en.wikipedia.org/wiki/Hardware_random_number_generator) (Wikipedia)


# Missing files?
Don't worry :slightly_smiling_face: Just hit Alt-K to generate /Drivers/CMCIS/ and /Drivers/STM32G4xx_HAL_Driver/ based on the .ioc file. After a couple of seconds your project will be ready for building.

# Libraries
* OLED: [stm32-ssd1306](https://github.com/afiskon/stm32-ssd1306) (MIT license)

# Software tools
* [RealTerm: Serial/TCP Terminal](https://sourceforge.net/projects/realterm/) - check the Capture tab :hammer_and_wrench:

![Capture tab](/Assets/Images/realterm_capture_tab.JPG)

# Exemplary hardware
* [Geiger counter](https://allegro.pl/oferta/detektor-promieniowania-jadrowego-licznik-geigera-15470959931)
* [Sound detector](https://sklep.msalamon.pl/produkt/mikrofon-z-czujnikiem-dzwieku-ky037/) (for a non-invasive integration)

# Readings and videos
* [Random Number Generating Geiger Counter](https://www.sparkfun.com/tutorials/132) (SparkFun)
* [Geiger–Muller tube](https://en.wikipedia.org/wiki/Geiger%E2%80%93M%C3%BCller_tube) (Wikipedia)
* [Arduino True Random Number Generator](https://www.instructables.com/Arduino-True-Random-Number-Generator/) (Instructables)
* [FNIRSI GC-01 [IMPORTANT PRODUCT UPDATE]](https://www.youtube.com/watch?v=E9lz3cr2KQs) (before you make your purchasing decisions) (Project 326)
* [The Perfect Cheap Geiger Counter?](https://www.youtube.com/watch?v=Nbz_Nbqgehc) (Project 326)
* [How to predict random numbers](https://www.youtube.com/watch?v=QzI6GFzH7ZA) (AlphaPhoenix)
* [FNIRSI Firmware Upgrade Review](https://www.youtube.com/watch?v=23sX-_Oo_h0) (Project 326)
* [Alpha particle](https://en.wikipedia.org/wiki/Alpha_particle) (Wikipedia)
* [Beta particle](https://en.wikipedia.org/wiki/Beta_particle) (Wikipedia)
* [Gamma ray](https://en.wikipedia.org/wiki/Gamma_ray) (Wikipedia)
* [X-ray](https://en.wikipedia.org/wiki/X-ray) (Wikipedia)
* [Uranium Radiation Properties](https://www.wise-uranium.org/rup.html) (WISE Uranium Project)
* [WISE (World Information Service on Energy)](https://www.wiseinternational.org/)
* [Radium Girls & the Failure of Unregulated Capitalism](https://www.youtube.com/watch?v=Y3jbY5NSVWU) (Meromorphic)
* [Bell's theorem](https://en.wikipedia.org/wiki/Bell%27s_theorem) (Wikipedia)

# The test suite (exemplary implementations)
* [A Statistical Test Suite for Random and Pseudorandom Number Generators for Cryptographic Applications](https://csrc.nist.gov/pubs/sp/800/22/r1/upd1/final) (NIST)
* [NIST Randomness Testsuit](https://github.com/stevenang/randomness_testsuite) (Python)
* [The NIST Statistical Test Suite](https://github.com/terrillmoore/NIST-Statistical-Test-Suite) (Euphoria)
* [NIST Statistical Test Suite](https://github.com/arcetri/sts) (C)
* [NIST SP 800-22 Statistical Test Suite](https://github.com/GINARTeam/NIST-statistical-test) (Python)
* [NIST Statistical Test Suite](https://github.com/kravietz/nist-sts) (C)
* [NIST-Tests](https://github.com/ArkapravaSain/NIST-Statistical-Test-Suite) (MATLAB)
* [NistRng](https://github.com/InsaneMonster/NistRng) (Python)
* [ENT — Fourmilab Random Sequence Tester](https://github.com/Fourmilab/ent_random_sequence_tester) (C, HTML)
* [NIST randomness test suite](https://github.com/ycmjason/nist-randomness-test-suite) (JavaScript)
* [EntropyAssessment](https://github.com/usnistgov/SP800-90B_EntropyAssessment) (C++)
* [nist-sts-ml](https://github.com/maltehi/nist-sts-ml) (MATLAB)
* [A variant of NIST SP 800-22 revison.1a test suit ](https://github.com/greendow/A-variant-of-NIST-SP-800-22-test-suit) (C)
* [TRNG-Test-Suite](https://github.com/BrooksOlney/TRNG-Test-Suite) (Python)
* [Random Bitstream Tester](https://github.com/mzsoltmolnar/random-bitstream-tester) (JavaScript, HTML)
* [Random Bitstream Tester](https://mzsoltmolnar.github.io/random-bitstream-tester/) (online tester)

The last one is perfect for newbies - you just ctrl+c ctrl+v the bitstream and hit the button. Note that some tests require at least one million of bits to be conclusive. I generated ca. [100 000 bits](/Assets/Data/geiger_rng_ca_100k_bits.txt). Half of the tests are flagged as erroneous due to the insufficient number of bits. The other half is passed.

![NIST test suite results](/Assets/Images/geiger_nist_tests.JPG)

I will stop tests here to proceed to other sources of entropy. Some of them happen to be quite attractive visually[^5] - I have to build one using STM32 (with HASH and DCMI peripherals) and OV2640. An ESP32 version has just been completed in the meantime of editing this README.md and it passes all the NIST tests without any lava lamps, just by staring at the celling. Apparently the CMOS image sensor noise does the necessary job. Nevertheless, I have to investigate it further before I can give any conclusions on it - saying anything with certainty about the randomness can be tricky :smile:

[^5]: [The Randomness Problem: How Lava Lamps Protect the Internet](https://www.youtube.com/watch?v=89EX1NF7eHQ) :volcano: + [Method for seeding a pseudo-random number generator with a cryptographic hash of a digitization of a chaotic system](https://patents.google.com/patent/US5732138)

# Call for action
Create your own [home laboratory/workshop/garage](http://ufnalski.edu.pl/control_engineering_for_hobbyists/2024_dzien_otwarty_we/Dzien_Otwarty_WE_2024_Control_Engineering_for_Hobbyists.pdf)! Get inspired by [ControllersTech](https://www.youtube.com/@ControllersTech), [DroneBot Workshop](https://www.youtube.com/@Dronebotworkshop), [Andreas Spiess](https://www.youtube.com/@AndreasSpiess), [GreatScott!](https://www.youtube.com/@greatscottlab), [ElectroBOOM](https://www.youtube.com/@ElectroBOOM), [Phil's Lab](https://www.youtube.com/@PhilsLab), [atomic14](https://www.youtube.com/@atomic14), [That Project](https://www.youtube.com/@ThatProject), [Paul McWhorter](https://www.youtube.com/@paulmcwhorter), and many other professional hobbyists sharing their awesome projects and tutorials! Shout-out/kudos to all of them!

> [!WARNING]
> Automation and control engineering - do try this at home :exclamation:

190+ challenges to start from: [Control Engineering for Hobbyists at the Warsaw University of Technology](http://ufnalski.edu.pl/control_engineering_for_hobbyists/Control_Engineering_for_Hobbyists_list_of_challenges.pdf).

Stay tuned!
