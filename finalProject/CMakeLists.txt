#---Adding example N3 subdirectories explicitly
#   and a custom target to for building all example N3 options ----------
cmake_minimum_required(VERSION 3.16...3.27)

add_subdirectory(Chamber)
add_subdirectory(MRWell)
add_subdirectory(GRWell)

add_custom_target(finalProject DEPENDS Chamber MRWell GRWell)
