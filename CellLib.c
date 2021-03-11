#define SOKOL_IMPL
#if defined(__APPLE__)
	#define SOKOL_METAL
#elif defined(_WIN32)
	#define SOKOL_D3D11
#elif defined(__linux)
	#define SOKOL_GLCORE33
#endif
#include "sokol/sokol_app.h"
#include "sokol/sokol_gfx.h"
#include "sokol/sokol_glue.h"
#include "CellLib.h"

struct CL CL = { 0 };

static void Initialize()
{
#if defined(__APPLE__)
	_sapp.macos.window.styleMask &= ~NSWindowStyleMaskResizable;
#endif
	sg_setup(&(sg_desc){ .context = sapp_sgcontext() });
	CL.AppInfo.Initialize();
}

static void Frame()
{
	CL.AppInfo.Frame();
	
	sg_pass_action pass =
	{
		.colors[0] = { .value = { 1.0f, 1.0f, 1.0f, 1.0f }, .action = SG_ACTION_CLEAR, }
	};
	sg_begin_default_pass(&pass, sapp_width(), sapp_height());
	sg_end_pass();
	sg_commit();
}

static void Deinitialize()
{
	CL.AppInfo.Deinitialize();
}

sapp_desc sokol_main(int argc, char* argv[])
{
	CL.AppInfo = CLMain(argc, argv);
	return (sapp_desc)
	{
		.init_cb = Initialize,
		.frame_cb = Frame,
		.cleanup_cb = Deinitialize,
		.width = CL.AppInfo.CellSize * CL.AppInfo.Width,
		.height = CL.AppInfo.CellSize * CL.AppInfo.Height,
		.window_title = CL.AppInfo.Title,
	};
}
