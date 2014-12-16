require('ash_helper')

ANSWER = 10
WINNER = 4

function initialize()
	-- create user buttons
	ash_helper.create_user_buttons('CORRECT', 'WRONG')

	-- create system buttons	
	ash_helper.create_system_buttons('FINISH')

	-- return info
	return {
		answer = ANSWER,
		winner = WINNER,
		title = '?th stage',
		subtitle = 'ƒWƒƒƒ“ƒ‹',
		quizid = 202,
		org_user = {
			correct = 0,
			wrong = 0,
			score = 0
		}
	}
end

function on_system_button(id)
	if id == 1 then	-- finish
		local users = ash_helper.get_all_users(ANSWER)
		ash_helper.sort_users(users)
		for i = 1, WINNER do ash.set_user(users[i].index, {}, {1}) end
	end
end

function on_user_button(index, id)
	local user, data = ash.get_user(index), {}

	if id == 1 then	-- correct
		data.correct = user.correct + 1
		data.score = user.score + 1
	elseif id == 2 then	-- wrong
		data.wrong = user.wrong + 1
		data.score = user.score - 1
	end

	ash.set_user(index, data)
end

