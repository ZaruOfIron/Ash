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
		org_user = ash_helper.all_zero_user
	}
end

function on_system_button(id)
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
		ash_helper.finish(ANSWER, WINNER)
	end
end

function on_user_button(index, id)
	if id == 1 then	-- correct
		table.insert(checked_users, index)
	end
end


