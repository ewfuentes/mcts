cc_library(
	name = "game",
	hdrs = ["game.hh",],
	visibility = ["//visibility:public",],
)

cc_library(
	name = "line_game",
    hdrs = ["line_game.hh",],
	srcs = ["line_game.cc",],
	deps = [
		":game",
	],
	visibility = ["//visibility:public",],
)

cc_library(
	name = "tic_tac_toe",
    hdrs = ["tic_tac_toe.hh",],
	srcs = ["tic_tac_toe.cc",],
	deps = [
		":game",
	],
	visibility = ["//visibility:public",],
)
