
option("with-tests")
    set_values(false, true)
    set_default(false)
    set_description("Build with tests")
    set_showmenu(true)
option_end()

option("test-framework")
    set_values("none", "boost")
    set_default("none")
    set_description("Test framework to use")
    set_showmenu(true)
option_end()
