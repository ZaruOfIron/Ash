require('ash_helper')

ANSWER = 11
WINNER = 3
CORRECT_LIMIT = 6
WRONG_LIMIT = 3
suc_correct, suc_wrong = {}, {}

function initialize()
	-- create user buttons
	ash_helper.create_user_buttons('CORRECT', 'WRONG')

	-- create system buttons	

	-- return info
	return {
		answer = ANSWER,
		winner = WINNER,
		title = '2nd stage',
		subtitle = 'Ｗ連答',
		quizid = 102,
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
		if suc_correct.index ~= index then	-- 初
			data.correct = user.correct + 1

			suc_correct.index = index
			suc_correct.count = 1
		else	-- 連
			data.correct = user.correct + 1 + suc_correct.count

			suc_correct.count = suc_correct.count + 1
			table.insert(info, 10201)
		end

		if data.correct >= CORRECT_LIMIT then	-- clear
			data.correct = CORRECT_LIMIT
			table.insert(info, 1)
		end
	elseif id == 2 then	-- wrong
		if suc_wrong.index ~= index then	-- 初
			data.wrong = user.wrong + 1

			suc_wrong.index = index
			suc_wrong.count = 1
		else	-- 連
			data.wrong = user.wrong + 1 + suc_wrong.count

			suc_wrong.count = suc_wrong.count + 1
			table.insert(info, 10202)
		end

		if data.wrong >= WRONG_LIMIT then	-- fail
			data.wrong = WRONG_LIMIT
			table.insert(info, 2)
		end
	end

	ash.set_user(index, data, info)
end


