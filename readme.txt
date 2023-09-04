plugin toolkit/framework

sketch, temporary:
(yeah, docs and proper presentation sorely needed!)

"initially the base code toolkit for all the SA (skei.audio) plugins, but now a bit more generic and hopefully useful for other projects and people.. 100% based on clap, with wrappers to and from other plugin formats (vst2/3, ladspa, lv2, standalone exe).. clap/ladspa hosting (other formats coming too, probably).. currently supports linux and windows, no mac version planned.. gpu-accelerated, freely scalable (vector) gui.. all clap goodies (poly modulation, voice stacking, thread pool, remote controls, preset discovery, parameter indication, sample accurate..), voice manager with threaded voices, overlapping notes, polyphonic modulation, etc, etc).. simplified, abstracted, streamlined for rapid installation/setup, prototyping, iterations.."

95% of the following is working, but is constantly being tweaked and improved..
the final bits and pieces is slowly being added..
whenever i (or potential contributors/collaborators) need it

* (download the toolkit, and have your first plugin up and running in a host within minutes)
* internal plugin format: clap (always present)
* clap-as-xxx wrappers: vst2, vst3, lv2, ladspa, exe
* xxx-as-clap: ladspa only at the moment, other formats planned
* linux (main target) + windows
* standalone executable: internal host + plugin(s)
* simple, readable, "old-school" c++
* no external dependencies (except a few system libs: x11/glx, gdi32/wgl, etc)
* no setup, initialization, configuration, linking, makefile/cmake mess
* compile one .cpp file, with one gcc/mingw command, resulting in one tiny binary file
* fluid and responsive, scalable, gpu accelerated gui
* advanced (?) hierarchial widget system
* tons of debugging helpers and tools
* lots of other helpful things, not directly plugin related, like compression, encryption, jit, vm, scripting, interpolation, easing, math approximations, etc, etc..
* headers-only
* lock/wait free

some previews: https://www.youtube.com/@torhelgeskei
see also: https://discord.gg/KMNea8P6Pe

