require('ash_helper')

ANSWER = 9
WINNER = 1
INITIAL_SCORE = 5
LIMIT_SCORE = 7
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

function on_system_button(id)
	if id == 1 then	-- finish
		ash_helper.finish(ANSWER, WINNER)
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
					ash.set_user(i, { score = user.score - 1 })
				end
			end
		end
	elseif id == 3 then	-- wrong
		data.wrong = user.wrong + 1
		data.score = user.score - 2
	end

	ash.set_user(index, data)
end

