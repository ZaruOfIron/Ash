require('ash_helper')

ANSWER = 9
WINNER = 1
CHALLENGE_SCORE = 5

now_challenging = false

function initialize()
	-- create user buttons
	ash_helper.create_user_buttons('CORRECT', 'WRONG')

	-- return info
	return {
		answer = ANSWER,
		winner = WINNER,
		title = '?th stage',
		subtitle = '’Ê‰ß••“›',
		quizid = 205,
		org_user = {
			correct = 0,
			wrong = 0,
			score = 0
		}
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
				now_challenging = true
			end
		end
	elseif id == 2 then	-- wrong
		data.wrong = user.wrong + 1

		if now_challenging then
			data.score = 3
			now_challenging = false
		else
			data.score = user.score - 1
		end
	end

	ash.set_user(index, data, info)
end

