CORRECT_LIMIT = 5
WRONG_LIMIT = 2
suc_index = 0

function initialize()
	-- create user buttons
	ash.config.create_user_button(1, 'CORRECT')
	ash.config.create_user_button(2, 'WRONG')

	-- create system buttons	

	-- return info
	return {
		answer = 12,
		winner = 5,
		title = '1st stage',
		subtitle = '‚T›‚Q~',
		quizid = 101,
		org_user = {
			correct = 0,
			wrong = 0,
			score = 0
		}
	}
end

function on_command(index, id)
	local user, data, info = ash.get_user(index), {}, {}

	if id == 1 then	-- correct
		data.correct = user.correct + 1

		if suc_index == index then	-- ˜A“š
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


