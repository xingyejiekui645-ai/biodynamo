/// Get the concentration at specified voxel
real_t DiffusionGrid::GetConcentration(const size_t idx) const {
  if (idx >= total_num_boxes_) {
    Log::Error("DiffusionGrid::GetConcentration",
               "You tried to get the concentration outside the bounds of "
               "the diffusion grid!");
    return 0;
  }
  return data_[idx];
}