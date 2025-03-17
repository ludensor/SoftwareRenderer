#pragma once

#include "core/sr_core_types.h"

class GraphicDevice;

struct DebugInfo
{
	Point position;
	std::wstring text;
};

class Application
{
public:
	Application();
	~Application();

	void Initialize();
	void Finalize();
	void Tick(float delta_time);

	const GraphicDevice& GetGraphicDevice() const;
	const std::vector<DebugInfo>& GetDebugInfos() const;

private:
	static constexpr int32_t DELTA_TIME_SAMPLE_COUNT = 60;

	GraphicDevice* graphic_device_;

	float delta_time_samples_[DELTA_TIME_SAMPLE_COUNT];
	int32_t next_sample_index_;

	std::vector<DebugInfo> debug_infos_;
};
