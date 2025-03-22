#include "sr_pch.h"
#include "core/sr_application.h"
#include "core/sr_graphic_device.h"
#include "shaders/sr_flat_shader.h"

Application::Application()
	: next_sample_index_(0)
	, debug_infos_(2)
{
	graphic_device_ = new GraphicDevice();

	std::fill_n(delta_time_samples_, DELTA_TIME_SAMPLE_COUNT, 0.016666f);

	debug_infos_[0].position = Point(10, 10);
	debug_infos_[1].position = Point(10, 30);
}

Application::~Application()
{
	delete graphic_device_;
}

void Application::Initialize()
{
	graphic_device_->Initialize();
}

void Application::Finalize()
{
	graphic_device_->Finalize();
}

void Application::Tick(float delta_time)
{
	graphic_device_->ClearPixelBuffer(math::Vector4(0.3f, 0.3f, 0.3f));
	graphic_device_->ClearDepthBuffer(1.0f);

	// Store the next delta time in the time sample array
	delta_time_samples_[next_sample_index_] = delta_time;
	next_sample_index_ = (next_sample_index_ + 1) % DELTA_TIME_SAMPLE_COUNT;

	float avg_delta_time = 0.0f;
	for (int32_t i = 0; i < DELTA_TIME_SAMPLE_COUNT; ++i)
	{
		avg_delta_time += delta_time_samples_[i];
	}
	avg_delta_time /= DELTA_TIME_SAMPLE_COUNT;

	// Frame per seconds
	const float fps = 1.0f / avg_delta_time;
	// Milliseconds per frame
	const float mspf = avg_delta_time * 1000.0f;

	debug_infos_[0].text = std::format(L"{:0.2f} FPS", fps);
	debug_infos_[1].text = std::format(L"{:0.2f} ms", mspf);

	IShader* shader = graphic_device_->GetShader();
	SR_ASSERT(shader);

	graphic_device_->Draw();
}

const GraphicDevice& Application::GetGraphicDevice() const
{
	SR_ASSERT(graphic_device_);
	return *graphic_device_;
}

const std::vector<DebugInfo>& Application::GetDebugInfos() const
{
	return debug_infos_;
}
