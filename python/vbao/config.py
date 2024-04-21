# This file is part of VBAO.
#
# VBAO is free software: you can redistribute it and/or modify it under the terms of
# the GNU Lesser General Public License as published by the Free Software Foundation,
# either version 3 of the License, or (at your option) any later version.
#
# VBAO is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License along with VBAO.
# If not, see <https://www.gnu.org/licenses/>.


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
