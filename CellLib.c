#define SOKOL_IMPL
#define SOKOL_GLCORE33
#include "sokol/sokol_app.h"
#include "sokol/sokol_gfx.h"
#include "sokol/sokol_glue.h"
#include "CellLib.h"

static CLAppInfo AppInfo;
static sg_buffer Quad;
static sg_shader Shader;
static sg_pipeline Pipeline;
static unsigned char * CellStagingBuffer;
static sg_image CellBuffer;

static void Initialize()
{
	AppInfo.Width = sapp_width() / AppInfo.CellSize;
	AppInfo.Height = sapp_height() / AppInfo.CellSize;
#if defined(__APPLE__)
	_sapp.macos.window.styleMask &= ~NSWindowStyleMaskResizable;
#endif
	sg_setup(&(sg_desc){ .context = sapp_sgcontext() });
	
	CellStagingBuffer = malloc(AppInfo.Width * AppInfo.Height);
	CLClear(CLColorWhite);
	
	sg_image_desc cellBufferInfo =
	{
		.width = AppInfo.Width,
		.height = AppInfo.Height,
		.type = SG_IMAGETYPE_2D,
		.usage = SG_USAGE_STREAM,
		.pixel_format = SG_PIXELFORMAT_R8,
	};
	CellBuffer = sg_make_image(&cellBufferInfo);
	
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
		
			"void main()"
			"{"
			"	gl_Position = vec4(VertexPosition, 0.0f, 1.0f);"
			"}",
		.fs.source =
			"#version 330\n"
			
			"uniform usampler2D Cells;"
			"uniform vec2 Viewport;"
			"layout (location = 0) out vec4 FinalColor;"
		
			"void main()"
			"{"
			"	vec2 uv = gl_FragCoord.xy / Viewport;"
			"	ivec2 size = textureSize(Cells, 0);"
			"	vec2 cellSize = Viewport / vec2(size);"
			"	vec2 cUV = mod(gl_FragCoord.xy, cellSize);"
			"	vec2 line = round(0.1f * cellSize);"
			"	if (cUV.x < line.x || cUV.y < line.y)"
			"	{"
			"		FinalColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);"
			"		return;"
			"	}"
			"	uint id = texture(Cells, uv).r;"
			"	vec3 color = vec3(uvec3(id >> 4, id >> 2, id >> 0) & 3u) / 3.0f;"
			"	FinalColor = vec4(color, 1.0f);"
			"}",
		.fs.images[0] =
		{
			.name = "Cells",
			.image_type = SG_IMAGETYPE_2D,
			.sampler_type = SG_SAMPLERTYPE_UINT,
		},
		.fs.uniform_blocks[0] =
		{
			.size = 2 * sizeof(float),
			.uniforms[0] = { .name = "Viewport", .type = SG_UNIFORMTYPE_FLOAT2 },
		},
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
	
	if (AppInfo.Initialize != NULL) { AppInfo.Initialize(); }
}

static void Frame()
{
	if (AppInfo.Update != NULL) { AppInfo.Update(); }
	
	sg_image_data data = { .subimage[0][0] = { .ptr = CellStagingBuffer, .size = AppInfo.Width * AppInfo.Height } };
	sg_update_image(CellBuffer, &data);
	sg_pass_action pass =
	{
		.colors[0] = { .value = { 1.0f, 1.0f, 1.0f, 1.0f }, .action = SG_ACTION_CLEAR, }
	};
	sg_begin_default_pass(&pass, sapp_width(), sapp_height());
	sg_apply_pipeline(Pipeline);
	sg_apply_bindings(&(sg_bindings){ .vertex_buffers[0] = Quad, .fs_images[0] = CellBuffer });
	sg_apply_uniforms(SG_SHADERSTAGE_FS, 0, &(sg_range){ .ptr = (float[]){ sapp_widthf(), sapp_heightf() }, .size = 2 * sizeof(float) });
	sg_draw(0, 6, 1);
	sg_end_pass();
	sg_commit();
}

static void Deinitialize()
{
	if (AppInfo.Deinitialize != NULL) { AppInfo.Deinitialize(); }
	sg_destroy_pipeline(Pipeline);
	sg_destroy_shader(Shader);
	sg_destroy_buffer(Quad);
	sg_destroy_image(CellBuffer);
	free(CellStagingBuffer);
	sg_shutdown();
}

sapp_desc sokol_main(int argc, char* argv[])
{
	AppInfo = CLMain(argc, argv);
	return (sapp_desc)
	{
		.init_cb = Initialize,
		.frame_cb = Frame,
		.cleanup_cb = Deinitialize,
		.width = AppInfo.CellSize * AppInfo.Width,
		.height = AppInfo.CellSize * AppInfo.Height,
		.window_title = AppInfo.Title,
		.swap_interval = 1,
	};
}

void CLSet(int x, int y, unsigned char c)
{
	if (x < 0 || y < 0 || x >= AppInfo.Width || y >= AppInfo.Height) { return; }
	CellStagingBuffer[y * AppInfo.Width + x] = c;
}

void CLSetAll(unsigned char * buffer)
{
	memcpy(CellStagingBuffer, buffer, AppInfo.Width * AppInfo.Height);
}

unsigned char CLGet(int x, int y)
{
	if (x < 0 || y < 0 || x >= AppInfo.Width || y >= AppInfo.Height) { return 0; }
	return CellStagingBuffer[y * AppInfo.Width + x];
}

void CLGetAll(unsigned char * buffer)
{
	memcpy(buffer, CellStagingBuffer, AppInfo.Width * AppInfo.Height);
}

void CLClear(unsigned char c)
{
	for (int x = 0; x < AppInfo.Width; x++)
	{
		for (int y = 0; y < AppInfo.Height; y++)
		{
			CellStagingBuffer[y * AppInfo.Width + x] = c;
		}
	}
}

int CLCellSize()
{
	return AppInfo.CellSize;
}

int CLWidth()
{
	return AppInfo.Width;
}

int CLHeight()
{
	return AppInfo.Height;
}
