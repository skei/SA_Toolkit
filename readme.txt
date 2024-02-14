[in progress]

yapf - yet another plugin framework
(c++ plugin development toolkit/framework)

* linux, win32
* clap-based (internal plugin format is clap, wrappers for other formats)
* clap, vst2, vst3, lv2, ladspa, standalone exe, (dssi)
* scalable/vector gui
* x11/win32, opengl/gles, nanovg, wayland

* sample accurate automation
* mono/polyphonic modulation
* note expressions, mpe
* voice stacking
* multi threaded voice/channel processing
* remote controls, parameter indication
* preset discovery
* ..

* header-only
* "sane c++", "orthodox c++", "c with classes", ..
* lock/wait-free (except a couple of atomic bools)
* rapid development (compile in seconds)
* few dependencies (just system things, like x11/opengl)
* no build system needed (one single compilation/translation unit)
* no installation, very little setup or configuration (just copy & compile)
* no bloat (cpu, ram or disk)
* no nonsense (focus on coding plugins, not fighting build systems or dependencies)

https://discord.gg/KMNea8P6Pe
https://www.youtube.com/@torhelgeskei
