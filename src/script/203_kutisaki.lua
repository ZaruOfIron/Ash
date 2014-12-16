require('ash_helper')

ANSWER = 10
WINNER = 4
add_score = 0

function initialize()
	-- create user buttons
	ash_helper.create_user_buttons('CORRECT', 'WRONG')

	-- create system buttons	
	ash_helper.create_system_buttons(
		'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'CLEAR', 'FINISH'
	)

	-- return info
	return {
		answer = ANSWER,
		winner = WINNER,
		title = '?th stage',
		subtitle = 'Œûæ',
		quizid = 203,
		org_user = ash_helper.all_zero_user
	}
end

function on_system_button(id)
	if id >= 1 and id <= 10 then	-- ”’l“ü—Í
		add_score = add_score * 10 + id % 10
	elseif id == 11 then	-- clear
		add_score = 0
	elseif id == 12 then	-- finish
		ash_helper.finish(ANSWER, WINNER)
	end
end

function on_user_button(index, id)
	local user, data = ash.get_user(index), {}

	if id == 1 then	-- correct
		data.correct = user.correct + 1
		data.score = user.score + add_score
	elseif id == 2 then	-- wrong
		data.wrong = user.wrong + 1
		data.score = user.score - add_score
	end

	ash.set_user(index, data)
	add_score = 0
end

