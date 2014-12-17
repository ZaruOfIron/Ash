require('ash_helper')

ANSWER = 11
WINNER = 3

function initialize()
	-- create user buttons
	ash_helper.create_user_buttons('CORRECT', 'ÇµÇËÇ∆ÇË', 'WRONG')

	-- create system buttons	
	ash_helper.create_system_buttons('FINISH')

	-- return info
	return {
		answer = ANSWER,
		winner = WINNER,
		title = '3rd Round 1st step First set',
		subtitle = 'ÉRÅ[ÉXÉø ENDLESS PARADE',
		quizid = 201,
		org_user = ash_helper.all_zero_user
	}
end

function export_save_data()
	return ''
end

function import_save_data(str)
end

function on_system_button(id)
	ash.save()

	if id == 1 then	-- finish
		ash_helper.finish(ANSWER, WINNER)
	end
end

function on_user_button(index, id)
	ash.save()

	local user, data, info = ash.get_user(index), {}, {}

	if id == 1 then	-- correct
		data.correct = user.correct + 1
		data.score = user.score + 1
	elseif id == 2 then	-- ÇµÇËÇ∆ÇË
		data.correct = user.correct + 1
		data.score = user.score + 2
		table.insert(info, 20101)
	elseif id == 3 then	-- wrong
		data.wrong = user.wrong + 1
		data.score = user.score - 1
	end

	ash.set_user(index, data, info)
end

