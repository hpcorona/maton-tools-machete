
var project = new Project("openal", "shared", "src");

project.addPlatforms(["android", "ios"]);
project.addConfigurations(["debug", "release"]);
project.addDefines(["AL_BUILD_LIBRARY", "AL_ALEXT_PROTOTYPES", "AL_LIBTYPE_DYNAMIC"]);
project.addIncludeDirs([".", "OpenAL32/include"]);

project.addFiles(["OpenAL32/alAuxEffectSlot.c",
	"OpenAL32/alBuffer.c",
	"OpenAL32/alEffect.c",
	"OpenAL32/alError.c",
	"OpenAL32/alExtension.c",
	"OpenAL32/alFilter.c",
	"OpenAL32/alListener.c",
	"OpenAL32/alSource.c",
	"OpenAL32/alState.c",
	"OpenAL32/alThunk.c",
	"Alc/ALc.c",
	"Alc/alcConfig.c",
	"Alc/alcDedicated.c",
	"Alc/alcEcho.c",
	"Alc/alcModulator.c",
	"Alc/alcReverb.c",
	"Alc/alcRing.c",
	"Alc/alcThread.c",
	"Alc/ALu.c",
	"Alc/bs2b.c",
	"Alc/helpers.c",
	"Alc/hrtf.c",
	"Alc/mixer.c",
	"Alc/panning.c",
	"Alc/backends/android.c",
	"Alc/backends/loopback.c",
	"Alc/backends/null.c"]);
//	"Alc/backends/opensl.c"]);
	
project.addLdLibs("log");
// CFlags: -ffast-math
project.addLdFlags("-Wl,-s");

Library.addProjects(project);
