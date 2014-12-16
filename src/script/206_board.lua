require('ash_helper')

ANSWER = 10
WINNER = 4

checked_users = {}

function initialize()
	-- create user buttons
	ash_helper.create_user_buttons('CHECK')

	-- create system buttons
	ash_helper.create_system_buttons('SUBMIT', 'CLEAR', 'FINISH')

	-- return info
	return {
		answer = ANSWER,
		winner = WINNER,
		title = '?th stage',
		subtitle = 'ボード',
		quizid = 206,
		org_user = {
			correct = 0,
			wrong = 0,
			score = 0
		}
	}
end

function on_command(index, id)
	if index == 0 then	-- system
		if id == 1 then	-- submit
			-- 追加点を決定
			local user_count = table.maxn(checked_users)
			local add_score = 0
			if user_count == 1 then add_score = 3
			elseif user_count >= 2 and user_count <= 3 then add_score = 2
			else add_score = 1 end

			for i = 1, ANSWER do
				local user = ash.get_user(i)
				if ash_helper.search_array(checked_users, i) then	-- correct
					ash.set_user(i, { correct = user.correct + 1, score = user.score + add_score })
				else	-- wrong
					ash.set_user(i, { wrong = user.wrong + 1 })
				end
			end

			checked_users = {}
		elseif id == 2 then	-- clear
			checked_users = {}
		elseif id == 3 then	-- finish
			local users = ash_helper.get_all_users(ANSWER)
			ash_helper.sort_users(users)
			for i = 1, WINNER do
				ash.set_user(users[i].index, {}, {1})
			end
		end
	else	-- user
		if id == 1 then	-- correct
			table.insert(checked_users, index)
		end
	end
end

