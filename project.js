
var project = new Project("machete", "whole_static", "machete");

project.addPlatforms(["android", "ios"]);
project.addConfigurations(["debug", "release"]);
project.addDependencies(["zip", "png", "tremor", "openal"]);
project.addIncludeDirs(".");

project.addFiles(["machete/widget/extended.cpp",
	"machete/widget/widget.cpp",
	"machete/thread/thread.cpp",
	"machete/storage/sqlite3.c",
	"machete/storage/sql.cpp",
	"machete/sound/sound.cpp",
	"machete/sound/backend/backend.cpp",
	"machete/platform/platform.cpp",
	"machete/input/touch.cpp",
	"machete/graphics/draw.cpp",
	"machete/graphics/shader.cpp",
	"machete/draw/bundle.cpp",
	"machete/draw/element.cpp",
	"machete/draw/font.cpp",
	"machete/draw/meta.cpp",
	"machete/draw/asha.cpp",
	"machete/data/mbd.cpp",
	"machete/common/callback.cpp",
	"machete/anim/func.cpp",
	"machete/anim/tween.cpp"]);
	
project.addLdLibs(["GLESv2", "dl", "log", "z"]);

project.addConfigurator("android", null, function(_proj) {
	_proj.addDefines("TARGET_ANDROID");
	_proj.addFiles(["machete/android/com_maton_machete_MacheteNative.cpp",
		"machete/sound/backend/openal.cpp",
		"machete/android/utils.cpp",
		"machete/android/analytics.cpp",
		"machete/android/AndroidPlatform.cpp"]);
	_proj.addSpec("analytics_lib", "../deps/googleanalytics/bin/libGoogleAnalytics.jar");
	_proj.addSpec("java_sources", "../android");
});

project.addConfigurator("ios", null, function(_proj) {
	_proj.addDefines("TARGET_IOS");
	_proj.addFiles(["machete/sound/backend/openal.cpp"]);
});

project.addConfigurator(null, "debug", function(_proj) {
	_proj.addDefines("DEBUG");
});

Library.addProjects(project);
Library.load(["deps/libopenal", "deps/libpng", "deps/libtremor", "deps/libzip"]);
