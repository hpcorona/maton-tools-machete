
var project = new Project("tremor", "static", ".");

project.addPlatforms(["android", "ios"]);
project.addConfigurations(["debug", "release"]);
project.addDefines("HAVE_ALLOCA_H");
project.addIncludeDirs(".");

project.addFiles(["tremor/bitwise.c",
	"tremor/block.c",
	"tremor/codebook.c",
	"tremor/floor0.c",
	"tremor/floor1.c",
	"tremor/framing.c",
	"tremor/info.c",
	"tremor/mapping0.c",
	"tremor/mdct.c",
	"tremor/registry.c",
	"tremor/res012.c",
	"tremor/sharedbook.c",
	"tremor/synthesis.c",
	"tremor/vorbisfile.c",
	"tremor/window.c"]);

Library.addProjects(project);
