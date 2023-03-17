
target("QtTest")
    add_rules("qt.widgetapp")
    set_kind("binary")

    add_files("qt-test/*.cpp")
    add_files("qt-test/view.ui")
    add_files("qt-test/view.h")

target("VBAOTest")
    add_rules("qt.widgetapp")
    set_kind("binary")
    add_includedirs("$(scriptdir)/Lib_VBAO")
    add_includedirs("$(scriptdir)")

    add_files("model/*.cpp","model/*.hpp")
    add_files("viewmodel/*.cpp","viewmodel/*.hpp")
    add_files("view/*.cpp","view/displayer.h","view/displayer.ui")
    add_files("window/*.cpp","window/*.hpp")
    add_files("app/*.cpp","app/*.hpp","app/*.h")

target("VBAO")
    set_installdir("$(buildir)/install")
    add_installfiles("Lib_VBao/COPYING.LESSER",{prefixdir = "include"})
    add_installfiles("Lib_VBao/VBAO/*",{prefixdir = "include/VBAO"})
    set_languages("cxx17")
    set_kind("headeronly")

--package("libVBAO")
--    set_description("VBAO, a light lib for building MVVM project")
--    add_deps("QtTest","VBAOTest")
--    add_deps("VBAO")
