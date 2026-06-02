
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
    add_includedirs("$(scriptdir)/example")

    add_files("example/app/main.cpp")
    add_files("example/model/*.[hc]pp")
    add_files("example/viewmodel/*.*pp")
    add_files("example/view/*.*pp","example/view/displayer.ui")
    add_files("example/window/*.*pp")
    add_files("example/app/*.*pp","example/app/*.h")

target("VBAO")
    set_installdir("$(buildir)/install")
    add_installfiles("Lib_VBao/COPYING.LESSER",{prefixdir = "VBAO"})
    add_installfiles("Lib_VBao/VBAO/*",{prefixdir = "VBAO/include"})
    set_languages("cxx17")
    set_kind("headeronly")

--package("libVBAO")
--    set_description("VBAO, a light lib for building MVVM project")
--    add_deps("QtTest","VBAOTest")
--    add_deps("VBAO")
