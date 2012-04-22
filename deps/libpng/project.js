
var project = new Project("png", "static", "src");

project.addPlatforms(["android", "ios"]);
project.addConfigurations(["debug", "release"]);
project.addIncludeDirs(".");

project.addFiles(["png.c",
	"pngerror.c",
	"pngget.c",
	"pngmem.c",
	"pngpread.c",
	"pngread.c",
	"pngrio.c",
	"pngrtran.c",
	"pngrutil.c",
	"pngset.c",
	"pngtrans.c",
	"pngwio.c",
	"pngwrite.c",
	"pngwtran.c",
	"pngwutil.c"]);
	
project.addLdLibs("z");

Library.addProjects(project);
