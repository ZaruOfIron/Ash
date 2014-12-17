require('ash_helper')

ANSWER = 12
WINNER = 5
CORRECT_LIMIT = 5
WRONG_LIMIT = 2
suc_index = 0

function initialize()
	-- create user buttons
	ash_helper.create_user_buttons('CORRECT', 'WRONG')

	-- create system buttons
	ash_helper.create_system_buttons('FINISH')

	-- return info
	return {
		answer = ANSWER,
		winner = WINNER,
		title = '2nd Round �D����ڎw����',
		subtitle = '�T���Q�~�A������',
		quizid = 101,
		org_user = ash_helper.all_zero_user
	}
end

function on_system_button(id)
	if id == 1 then	-- finish
		ash_helper.finish(ANSWER, WINNER)
	end
end

function on_user_button(index, id)
	local user, data, info = ash.get_user(index), {}, {}

	if id == 1 then	-- correct
		data.correct = user.correct + 1

		if suc_index == index then	-- �A��
			data.correct = data.correct + 1
			table.insert(info, 10101)
		else
			suc_index = index
		end

		if data.correct >= CORRECT_LIMIT then	-- clear
			data.correct = CORRECT_LIMIT
			table.insert(info, 1)
		end
	elseif id == 2 then	-- wrong
		data.wrong = user.wrong + 1

		if suc_index == index then
			suc_index = 0
		end

		if data.wrong >= WRONG_LIMIT then	-- fail
			data.wrong = WRONG_LIMIT
			table.insert(info, 2)
		end
	end

	ash.set_user(index, data, info)
end


