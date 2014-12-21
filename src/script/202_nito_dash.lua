require('ash_helper')

ANSWER = 10
WINNER = 4

function export_save_data()
	return ''
end

function import_save_data(str)
end

function initialize()
	-- create user buttons
	ash_helper.create_user_buttons('1 CORRECT', '2 CORRECT', '1 WRONG', '2 WRONG')

	-- create system buttons	
	ash_helper.create_system_buttons('FINISH')

	-- return info
	return {
		answer = ANSWER,
		winner = WINNER,
		title = '3rd Round Final step Quarter final',
		subtitle = 'ÉRÅ[ÉXÉ¿ Ç…Ç¡Ç±Ç…Ç¡Ç±Ç…Å`ÅÙ',
		quizid = 202,
		org_user = ash_helper.all_zero_user
	}
end

function on_system_button(id)
	ash.save()

	if id == 1 then	-- finish
		ash_helper.finish(ANSWER, WINNER)
	end
end

function on_user_button(index, id)
	ash.save()

	local user, data = ash.get_user(index), {}

	if id == 1 then	-- 1 correct
		data.correct = user.correct + 1
		data.score = user.score + 1
	elseif id == 2 then	-- 2 correct
		data.correct = user.correct + 1
		data.score = user.score + 3
	elseif id == 3 then	-- 1 wrong
		data.wrong = user.wrong + 1
		data.score = user.score - 1
	elseif id == 4 then	-- 2 wrong
		data.wrong = user.wrong + 1
		data.score = user.score - 2
	end

	ash.set_user(index, data)
end

