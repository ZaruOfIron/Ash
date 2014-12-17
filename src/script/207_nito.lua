require('ash_helper')

ANSWER = 10
WINNER = 4
answer_count = 0

function initialize()
	-- create user buttons
	ash_helper.create_user_buttons('CORRECT', 'WRONG')

	-- create system buttons	
	ash_helper.create_system_buttons('FINISH')

	-- return info
	return {
		answer = ANSWER,
		winner = WINNER,
		title = '3rd Round 3rd step Quarter final',
		subtitle = 'コースβ にっこにっこにー',
		quizid = 207,
		org_user = ash_helper.all_zero_user
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

		if answer_count == 0 then	-- 一着
			data.score = user.score + 2
			table.insert(info, 20701)
		else	-- 二着
			data.score = user.score + 1
			table.insert(info, 20702)
		end
	elseif id == 2 then	-- wrong
		data.wrong = user.wrong + 1

		if answer_count == 0 then	-- 一着
			data.score = user.score - 1
		else	-- 二着
			data.score = user.score
		end
	end

	answer_count = (answer_count + 1) % 2

	ash.set_user(index, data, info)
end

