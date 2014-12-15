ANSWER = 10
WINNER = 4
add_score = 0

function initialize()
	-- create user buttons
	ash.config.create_user_button(1, 'CORRECT')
	ash.config.create_user_button(2, 'WRONG')

	-- create system buttons	
	ash.config.create_system_button(1, '1')
	ash.config.create_system_button(2, '2')
	ash.config.create_system_button(3, '3')
	ash.config.create_system_button(4, '4')
	ash.config.create_system_button(5, '5')
	ash.config.create_system_button(6, '6')
	ash.config.create_system_button(7, '7')
	ash.config.create_system_button(8, '8')
	ash.config.create_system_button(9, '9')
	ash.config.create_system_button(10, '0')
	ash.config.create_system_button(11, 'CLEAR')
	ash.config.create_system_button(12, 'FINISH')

	-- return info
	return {
		answer = ANSWER,
		winner = WINNER,
		title = '?th stage',
		subtitle = '口先',
		quizid = 203,
		org_user = {
			correct = 0,
			wrong = 0,
			score = 0
		}
	}
end

function on_command(index, id)
	if index == 0 then	-- system
		if id >= 1 and id <= 10 then	-- 数値入力
			add_score = add_score * 10 + id % 10
		elseif id == 11 then	-- clear
			add_score = 0
		elseif id == 12 then	-- finish
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
		local user, data, info = ash.get_user(index), {}, {}

		if id == 1 then	-- correct
			data.correct = user.correct + 1
			data.score = user.score + add_score
		elseif id == 2 then	-- wrong
			data.wrong = user.wrong + 1
			data.score = user.score - add_score
		end

		ash.set_user(index, data, info)
		add_score = 0
	end
end

