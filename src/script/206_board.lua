ANSWER = 10
WINNER = 4

checked_users = {}

function search_array(array, target)
	for idx, value in pairs(array) do
		if value == target then return idx end
	end
	return nil
end

function initialize()
	-- create user buttons
	ash.config.create_user_button(1, 'CHECK')

	-- create system buttons	
	ash.config.create_system_button(1, 'SUBMIT')
	ash.config.create_system_button(2, 'CLEAR')
	ash.config.create_system_button(3, 'FINISH')

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
			local user_count = table.maxn(checked_users)
			local add_score = 0
			if user_count == 1 then add_score = 3
			elseif user_count >= 2 and user_count <= 3 then add_score = 2
			else add_score = 1 end

			for i = 1, ANSWER do
				local user = ash.get_user(i)
				if search_array(checked_users, i) then	-- correct
					ash.set_user(i, { correct = user.correct + 1, score = user.score + add_score })
				else	-- wrong
					ash.set_user(i, { wrong = user.wrong + 1 })
				end
			end

			checked_users = {}
		elseif id == 2 then	-- clear
			checked_users = {}
		elseif id == 3 then	-- finish
			-- userを全部取得する
			local users = {}
			for i = 1, ANSWER do
				table.insert(users, ash.get_user(i))
			end
			
			-- クイズの規則に従ってソート
			-- scoreが高く、正答数が多く、誤答数が少なく、indexが小さいほう
			table.sort(users,
				function(a, b)
					if a.score ~= b.score then return a.score > b.score end
					if a.correct ~= b.correct then return a.correct > b.correct end
					if a.wrong ~= b.wrong then return a.wrong < b.wrong end
					return a.index < b.index
				end)

			-- うえからWINNER個は通過
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

