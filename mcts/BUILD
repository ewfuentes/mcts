cc_library(
    name = "game_tree",
    srcs = ["game_tree.hh"],
    deps = [
		"//game",
    ],
)

cc_binary(
	name = "game_tree_test",
	srcs = ["game_tree_test.cc"],
	deps = [
		":game_tree",
		"//game:line_game",
	]
)
