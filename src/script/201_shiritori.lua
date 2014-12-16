require('ash_helper')

ANSWER = 11
WINNER = 3

function initialize()
	-- create user buttons
	ash_helper.create_user_buttons('CORRECT', '‚µ‚è‚Æ‚è', 'WRONG')

	-- create system buttons	
	ash_helper.create_system_buttons('FINISH')

	-- return info
	return {
		answer = ANSWER,
		winner = WINNER,
		title = '2nd stage',
		subtitle = '‚µ‚è‚Æ‚è',
		quizid = 201,
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
		for i = 1, WINNER do
			ash.set_user(users[i].index, {}, {1})
		end
	end
end

function on_user_button(index, id)
	local user, data, info = ash.get_user(index), {}, {}

	if id == 1 then	-- correct
		data.correct = user.correct + 1
		data.score = user.score + 1
	elseif id == 2 then	-- ‚µ‚è‚Æ‚è
		data.correct = user.correct + 1
		data.score = user.score + 2
		table.insert(info, 20101)
	elseif id == 3 then	-- wrong
		data.wrong = user.wrong + 1
		data.score = user.score - 1
	end

	ash.set_user(index, data, info)
end

