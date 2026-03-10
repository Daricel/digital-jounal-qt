cmake_minimum_required(VERSION 3.21)
project(proiectqt31 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON) # Auto-MOC is usually handled by qt_add_executable/qt_wrap_ui/qt_add_library with Qt6
set(CMAKE_AUTORCC ON) # Auto-RCC
set(CMAKE_AUTOUIC ON) # Auto-UIC

# Find Qt6 packages
# Ensure 'Test' component is listed here, as it's needed for the main executable now.
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Test)

# ================ MAIN APPLICATION & INTEGRATED TESTS ================

# Define UI files
set(UI_FILES
        ui/mainwindow.ui
        ui/entrydialog.ui
        ui/removedialog.ui
        ui/filterdialog.ui
)

# Define ALL source files for the main application executable,
# including the source file for your tests (journalunifiedtest.cpp).
# This assumes you've moved the test implementations into 'journalunifiedtest.cpp'
# and created 'journalunifiedtest.h' as discussed.
set(SOURCE_FILES
        main.cpp
        model/journalentry.cpp
        repository/csvjournalrepo.cpp
        repository/jsonjournalrepo.cpp
        controller/controller.cpp
        controller/commands/addcommand.cpp
        controller/commands/updatecommand.cpp
        controller/commands/removecommand.cpp
        controller/filter/titlefilter.cpp
        controller/filter/contentfilter.cpp
        controller/filter/compositefilter.cpp
        ui/mainwindow.cpp
        ui/entrydialog.cpp
        ui/removedialog.cpp
        ui/filterdialog.cpp
        # Your test implementation file now becomes part of the main app sources
        journal_test/journalunifiedtest.cpp # <-- New: Add the test implementation file here
)

# Define ALL header files that contain Q_OBJECT for MOC processing.
# These will be automatically MOC'ed by qt_add_executable if CMAKE_AUTOMOC is ON,
# but it's good to be explicit for clarity or if issues arise.
set(Q_OBJECT_HEADERS
        model/journalentry.h
        repository/csvjournalrepo.h
        repository/jsonjournalrepo.h
        controller/controller.h
        controller/commands/addcommand.h
        controller/commands/command.h # If it has Q_OBJECT
        controller/commands/updatecommand.h
        controller/commands/removecommand.h
        controller/filter/filter.h # If it has Q_OBJECT
        controller/filter/titlefilter.h
        controller/filter/contentfilter.h
        controller/filter/compositefilter.h
        ui/mainwindow.h
        ui/entrydialog.h
        ui/removedialog.h
        ui/filterdialog.h
        # Add your test class header here if it contains Q_OBJECT
        journal_test/journalunifiedtest.h # <-- New: Header for your test class
        # Add other headers with Q_OBJECT if they exist (e.g., sample_entries.h, app_styles.h if they have Q_OBJECT)
)

# Process UI files (still needed for your UI)
qt_wrap_ui(UI_HEADERS ${UI_FILES})

# Create the main executable using Qt's helper function
# This will also handle MOC, RCC, UIC automatically for the listed sources and headers.
qt_add_executable(proiectqt31
        ${SOURCE_FILES}
        ${UI_HEADERS}
        ${Q_OBJECT_HEADERS} # Pass headers for explicit MOC processing if needed, though AUTOMOC often covers this.
)

# Define include directories so the compiler can find all your header files.
target_include_directories(proiectqt31 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/model
        ${CMAKE_CURRENT_SOURCE_DIR}/repository
        ${CMAKE_CURRENT_SOURCE_DIR}/controller
        ${CMAKE_CURRENT_SOURCE_DIR}/ui
        ${CMAKE_CURRENT_SOURCE_DIR}/journal_test # <-- New: Add the directory containing your test header/source
        ${CMAKE_CURRENT_BINARY_DIR} # For generated files like ui_*.h
)

# Link the main executable with all necessary Qt modules.
# !!! Crucial: Link with Qt6::Test to use QTest::qExec() from main().
target_link_libraries(proiectqt31 PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Test # <-- Crucial: Now linked to your main executable
)

# Enable symbol exports if needed (usually for libraries, but harmless here)
set_target_properties(proiectqt31 PROPERTIES ENABLE_EXPORTS ON)

# ================ REMOVE SEPARATE TEST EXECUTABLE ================
# The previous 'TESTS' block is now removed because tests are integrated.
# This means 'add_executable(test_journal_unified ...)' and related settings are gone.
# CTest integration (add_test, enable_testing) is also removed unless you want to
# define a custom CTest entry that runs your main app with the --run-tests argument.
# For simplicity, we'll remove it here. If you need CTest, that would be a separate setup.