require('ash_helper')

ANSWER = 9
WINNER = 1
INITIAL_SCORE = 5
LIMIT_SCORE = 6
CHARGE_COUNT_LIMIT = 3

charge_count = {}

function initialize()
	-- create user buttons
	ash_helper.create_user_buttons('CHARGE', 'ATTACK', 'WRONG')

	-- create system buttons	
	ash_helper.create_system_buttons('FINISH')

	-- return info
	return {
		answer = ANSWER,
		winner = WINNER,
		title = '3rd Round 1st step',
		subtitle = 'Final set Shangri-La Shower',
		quizid = 204,
		org_user = {
			correct = 0,
			wrong = 0,
			score = INITIAL_SCORE
		}
	}
end

function on_system_button(id)
	if id == 1 then	-- finish
		local users = ash_helper.get_all_users(ANSWER)
		table.sort(users,
			function(a, b)
				if a.score ~= b.score then return a.score > b.score end
				if a.correct ~= b.correct then return a.correct > b.correct end
				return a.index < b.index
			end)
		for i = 1, WINNER do
			ash.set_user(users[i].index, {}, {1})
		end
	end
end

function on_user_button(index, id)
	local user, data = ash.get_user(index), {}

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
					local s = user.score - 1
					if s <= 0 then
						ash.set_user(i, { score = 0 }, {2})
					else
						ash.set_user(i, { score = s })
					end
				end
			end
		end
	elseif id == 3 then	-- wrong
		data.wrong = user.wrong + 1
		data.score = user.score - 2

		if data.score <= 0 then
			data.score = 0
			ash.set_user(index, data, {2})
		end
	end

	
	ash.set_user(index, data)
end

