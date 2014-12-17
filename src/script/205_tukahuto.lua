require('ash_helper')

ANSWER = 9
WINNER = 1
CHALLENGE_SCORE = 5

now_challenging = false
seed_indexes = {}

function initialize()
	-- create user buttons
	ash_helper.create_user_buttons('CORRECT', 'WRONG')

	-- return info
	return {
		answer = ANSWER,
		winner = WINNER,
		title = '3rd Round 2nd step',
		subtitle = 'Final set 通過ぁ？ミトメラレナイワァ!!',
		quizid = 205,
		org_user = ash_helper.all_zero_user
	}
end

function on_user_button(index, id)
	local user, data, info = ash.get_user(index), {}, {}

	if id == 1 then	-- correct
		data.correct = user.correct + 1

		if now_challenging then
			table.insert(info, 1)
			now_challenging = false
		else
			data.score = user.score + 1
			if data.score >= CHALLENGE_SCORE then
				if ash_helper.search_array(seed_indexes, index) then
					table.insert(info, 1)
				else
					now_challenging = true
				end
			end
		end
	elseif id == 2 then	-- wrong
		data.wrong = user.wrong + 1

		if now_challenging then
			data.score = CHALLENGE_SCORE - 2
			now_challenging = false
			table.insert(seed_indexes, index)
		else
			data.score = user.score - 1
		end

		if data.wrong >= 3 then
			table.insert(info, 2)
		end
	end

	ash.set_user(index, data, info)
end

