require('ash_helper')

ANSWER = 11
WINNER = 3
CORRECT_LIMIT = 6
WRONG_LIMIT = 3

suc_correct, suc_wrong = {}, {}
suc_correct_info, suc_wrong_indexes = {}, {}

function initialize()
	-- create user buttons
	ash_helper.create_user_buttons('CORRECT', 'WRONG')

	-- create system buttons	
	ash_helper.create_system_buttons('FINISH')

	-- return info
	return {
		answer = ANSWER,
		winner = WINNER,
		title = '3rd Round 1st step',
		subtitle = 'コースβ',
		quizid = 102,
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
		if suc_correct_info.index ~= index then	-- 初
			data.correct = user.correct + 1

			suc_correct_info.index = index
			suc_correct_info.count = 1

			local target = ash_helper.search_array(suc_wrong_indexes, index)
			if target ~= nil then
				table.remove(suc_wrong_indexes, target)
			end
		else	-- 連
			data.correct = user.correct + 1 + suc_correct_info.count

			suc_correct_info.count = suc_correct_info.count + 1
			table.insert(info, 10201)
		end

		if data.correct >= CORRECT_LIMIT then	-- clear
			data.correct = CORRECT_LIMIT
			table.insert(info, 1)
		end
	elseif id == 2 then	-- wrong
		if ash_helper.search_array(suc_wrong_indexes, index) then	-- 連誤答
			data.wrong = user.wrong + 2
		else
			data.wrong = user.wrong + 1
			table.insert(suc_wrong_indexes, index)
		end

		if data.wrong >= WRONG_LIMIT then	-- fail
			data.wrong = WRONG_LIMIT
			table.insert(info, 2)
		end

	end

	ash.set_user(index, data, info)
end


