plugin toolkit/framework

---

[todo, sketch]

initially the base code toolkit for all the SA (skei.audio) plugins, but now a bit more generic and hopefully useful for other projects and people.. 100% based on clap, with wrappers to and from other plugin formats (vst2, vst3, ladspa, standalone exe).. clap/ladspa hosting (vst2/3 coming, probably).. currently supports linux and windows, no mac version planned.. gpu-accelerated, freely scalable (vector) gui.. all clap goodies (modulation, voice stacking, thread pool, remote controls, preset discovery, parameter indication, sample accurate..), voice manager with threaded voices, overlapping notes, polyphonic modulation, etc, etc).. simplified, abstracted, streamlined for rapid installation/setup, prototyping, iterations..

no makefile or other weird build system or cmake monstrosities in sight, just one single .cpp file (your plugin) to compile (the toolkit is a collection of around 300 more or less free-standing header files), one simple gcc/mingw command line, no external dependencies (except a few system libs; x11, win32, opengl), no locks/waits/mutexes, no "modern c++" academic nonsense..

some previews: https://www.youtube.com/@torhelgeskei
see also: https://discord.gg/KMNea8P6Pe

if you have questions, or want more info, the most up-to-date version(s), assistance in any way, something specific implemented (or properly done/finished), support for certain features or platforms, help combining your own code/project with this (or the opposite way), modifications/tweaks, etc - contact me.. i can maybe help you turn your idea into a finished plugin.. :-)

