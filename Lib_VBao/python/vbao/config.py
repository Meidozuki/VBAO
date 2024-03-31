


qt_installed = None
# find out if Qt is installed because Qt's property will conflict with VBAO
try:
    from PIL import ImageQt
    qt_installed = ImageQt.qt_is_installed
except ImportError:
    pass

if qt_installed is None:
    qt_installed = False
    # PIL uninstalled, manually try import Qt
    for qt_module in ["PySide6", "PyQt6", "PySide2", "PySide2"]:
        try:
            if qt_module == "PySide6":
                import PySide6
            elif qt_module == "PyQt6":
                import PyQt6
            elif qt_module == "PySide2":
                import PySide2
            elif qt_module == "PyQt5":
                import PyQt5
        except ImportError:
            continue

        qt_installed = True
        break
