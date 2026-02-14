//#include "Pipeline.h"
//
//
//void Atlas::Pipeline::setBindPoint(EBindPoint newBindPoint) {
//	mBindPoint = newBindPoint;
//}
//
//Atlas::Pipeline::Pipeline(EBindPoint bindPoint, VkPipeline pipeline, PipelineLayout pipelineLayout) 
//	: AVulkanHandleWrapper(pipeline), mBindPoint(bindPoint), mPipelineLayout(pipelineLayout) {
//}
//
//void Atlas::Pipeline::bind(CommandBuffer commandBuffer) {
//	vkCmdBindPipeline(commandBuffer, (VkPipelineBindPoint)mBindPoint, getHandle());
//}
//
//Atlas::Pipeline::EBindPoint Atlas::Pipeline::getBindPoint() const noexcept
//{
//	return mBindPoint;
//}
