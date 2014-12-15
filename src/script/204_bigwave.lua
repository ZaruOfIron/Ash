ANSWER = 9
WINNER = 1
INITIAL_SCORE = 5
LIMIT_SCORE = 7
CHARGE_COUNT_LIMIT = 3

charge_count = {}

function initialize()
	-- create user buttons
	ash.config.create_user_button(1, 'CHARGE')
	ash.config.create_user_button(2, 'ATTACK')
	ash.config.create_user_button(3, 'WRONG')

	-- create system buttons	
	ash.config.create_system_button(1, 'FINISH')

	-- return info
	return {
		answer = ANSWER,
		winner = WINNER,
		title = '?th stage',
		subtitle = 'Big Wave',
		quizid = 204,
		org_user = {
			correct = 0,
			wrong = 0,
			score = INITIAL_SCORE
		}
	}
end

function on_command(index, id)
	if index == 0 then	-- system
		if id == 1 then	-- finish
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

		if id == 1 then -- charge
			if charge_count[index] == nil then charge_count[index] = 0 end
			if charge_count[index] >= CHARGE_COUNT_LIMIT then return end
			charge_count[index] = charge_count[index] + 1

			data.correct = user.correct + 1
			data.score = user.score + 2
			if data.score >= LIMIT_SCORE then data.score = LIMIT_SCORE end
		elseif id == 2 then	-- attack
			data.correct = user.correct + 1

			-- 全てのユーザーの得点を-1
			-- ただし、indexと等しいまたはget_userでnilが返ってきた場合を除く
			-- continue文がないためにifが数珠繋ぎになっている
			for i = 1, ANSWER do
				if i ~= index then
					local user = ash.get_user(i)
					if user ~= nil then
						ash.set_user(i, { score = user.score - 1 })
					end
				end
			end
		elseif id == 3 then	-- wrong
			data.wrong = user.wrong + 1
			data.score = user.score - 2
		end

		ash.set_user(index, data, info)
	end
end

