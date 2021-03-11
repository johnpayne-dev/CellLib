#define SOKOL_IMPL
#define SOKOL_GLCORE33
#include "sokol/sokol_app.h"
#include "sokol/sokol_gfx.h"
#include "sokol/sokol_glue.h"
#include "CellLib.h"

struct CL CL = { 0 };

static sg_buffer Quad;
static sg_shader Shader;
static sg_pipeline Pipeline;

static void Initialize()
{
#if defined(__APPLE__)
	_sapp.macos.window.styleMask &= ~NSWindowStyleMaskResizable;
#endif
	sg_setup(&(sg_desc){ .context = sapp_sgcontext() });
	
	sg_buffer_desc quadInfo =
	{
		.type = SG_BUFFERTYPE_VERTEXBUFFER,
		.usage = SG_USAGE_IMMUTABLE,
		.data =
		{
			.ptr = (float[]){ -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f },
			.size = 6 * (2 * sizeof(float)),
		}
	};
	Quad = sg_make_buffer(&quadInfo);
	
	sg_shader_desc shaderInfo =
	{
		.vs.source =
			"#version 330\n"
		
			"layout (location = 0) in vec2 VertexPosition;"
			"out vec2 FragUV;"
		
			"void main()"
			"{"
			"	FragUV = VertexPosition * 0.5f + 0.5f;"
			"	gl_Position = vec4(VertexPosition, 0.0f, 1.0f);"
			"}",
		.fs.source =
			"#version 330\n"
			
			"in vec2 FragUV;"
			"layout (location = 0) out vec4 FinalColor;"
		
			"void main()"
			"{"
				"FinalColor = vec4(FragUV, 1.0f, 1.0f);"
			"}",
	};
	Shader = sg_make_shader(&shaderInfo);
	
	sg_pipeline_desc pipelineInfo =
	{
		.shader = Shader,
		.layout =
		{
			.attrs = { [0].format = SG_VERTEXFORMAT_FLOAT2, }
		}
	};
	Pipeline = sg_make_pipeline(&pipelineInfo);
	
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
	sg_apply_pipeline(Pipeline);
	sg_apply_bindings(&(sg_bindings){ .vertex_buffers[0] = Quad });
	sg_draw(0, 6, 1);
	sg_end_pass();
	sg_commit();
}

static void Deinitialize()
{
	CL.AppInfo.Deinitialize();
	sg_destroy_pipeline(Pipeline);
	sg_destroy_shader(Shader);
	sg_destroy_buffer(Quad);
	sg_shutdown();
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
