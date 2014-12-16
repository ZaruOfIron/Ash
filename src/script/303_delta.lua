require('ash_helper')

ANSWER = 3
SCORE_LIMIT = 5

dir_count = 0
suc_index = 0

function initialize()
	-- create user buttons
	ash_helper.create_user_buttons('CORRECT', 'WRONG')

	-- return info
	return {
		answer = ANSWER,
		winner = 1,
		title = '?th stage',
		subtitle = '�W������',
		quizid = 202,
		org_user = ash_helper.all_zero_user
	}
end

function on_user_button(index, id)
	local user, data = ash.get_user(index), {}

	if id == 1 then	-- correct
		data.correct = user.correct + 1
		data.score = user.score + 1

		if suc_index == index then	-- �A��
			dir_count = (dir_count + 1) % 2
		else
			suc_index = index
		end

		if data.score >= SCORE_LIMIT then	-- clear
			table.insert(info, 1)
		end
	elseif id == 2 then	-- wrong
		data.wrong = user.wrong + 1

		local target = ash.get_user((index + dir_count) % ANSWER + 1)
		if target.score + 1 >= SCORE_LIMIT then -- clear
			ash.set_user(target.index, { score = target.score + 1 }, {1})
		else
			ash.set_user(target.index, { score = target.score + 1 })
		end
	end

	ash.set_user(index, data)
end
