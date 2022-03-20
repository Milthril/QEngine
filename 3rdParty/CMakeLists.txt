add_subdirectory(assimp)
add_subdirectory(KDDockWidgets)
add_subdirectory(QScintilla)

add_library(Imgui
    STATIC
    Imgui/imconfig.h
    Imgui/imgui.cpp
    Imgui/imgui.h
    Imgui/imgui_demo.cpp
    Imgui/imgui_draw.cpp
    Imgui/imgui_internal.h
    Imgui/imgui_widgets.cpp
    Imgui/imgui_tables.cpp
    Imgui/imstb_rectpack.h
    Imgui/imstb_textedit.h
    Imgui/imstb_truetype.h
)
target_include_directories(Imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Imgui)

add_library(ImGuizmo
    STATIC
    ImGuizmo/GraphEditor.h
    ImGuizmo/GraphEditor.cpp
    ImGuizmo/GraphEditor.h
    ImGuizmo/GraphEditor.cpp
    ImGuizmo/ImGradient.cpp
    ImGuizmo/ImGradient.h
    ImGuizmo/ImGuizmo.cpp
    ImGuizmo/ImGuizmo.h
    ImGuizmo/ImSequencer.cpp
    ImGuizmo/ImSequencer.h
    ImGuizmo/ImZoomSlider.h
)
target_link_libraries(ImGuizmo PUBLIC Imgui)
target_include_directories(ImGuizmo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo)

function(get_all_targets var)
    set(targets)
    get_all_targets_recursive(targets ${CMAKE_CURRENT_SOURCE_DIR})
    set(${var} ${targets} PARENT_SCOPE)
endfunction()
macro(get_all_targets_recursive targets dir)
    get_property(subdirectories DIRECTORY ${dir} PROPERTY SUBDIRECTORIES)
    foreach(subdir ${subdirectories})
        get_all_targets_recursive(${targets} ${subdir})
    endforeach()
    get_property(current_targets DIRECTORY ${dir} PROPERTY BUILDSYSTEM_TARGETS)
    list(APPEND ${targets} ${current_targets})
endmacro()
get_all_targets(3rdTarget)

foreach(TARGET_ITEM ${3rdTarget})
    set_target_properties(${TARGET_ITEM} PROPERTIES FOLDER "3rdParty")
endforeach()