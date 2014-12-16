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
		title = '?th stage',
		subtitle = '２答',
		quizid = 207,
		org_user = {
			correct = 0,
			wrong = 0,
			score = 0
		}
	}
end

function on_command(index, id)
	if index == 0 then	-- system
		if id == 1 then	-- finish
			local users = ash_helper.get_all_users(ANSWER)
			ash_helper.sort_users(users)
			for i = 1, WINNER do
				ash.set_user(users[i].index, {}, {1})
			end
		end
	else	-- user
		local user, data, info = ash.get_user(index), {}, {}

		if id == 1 then	-- correct
			data.correct = user.correct + 1

			if answer_count == 0 then
				data.score = user.score + 2	-- 一着
				table.insert(info, 20701)
			else	-- 二着
				data.score = user.score + 1
				table.insert(info, 20702)
			end
		elseif id == 2 then	-- wrong
			data.wrong = user.wrong + 1

			data.score = user.score - 1
		end

		answer_count = (answer_count + 1) % 2

		ash.set_user(index, data, info)
	end
end

