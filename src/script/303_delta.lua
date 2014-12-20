require('ash_helper')

ANSWER = 3
SCORE_LIMIT = 17

dir_count = 0
suc_index = 0

function export_save_data()
	return ash_helper.serialize(ash_helper.create_twin_table('dir_count', 'suc_index'))
end

function import_save_data(str)
	local data = ash_helper.deserialize(str)
	dir_count = data.dir_count
	suc_index = data.suc_index
end

function initialize()
	-- create user buttons
	ash_helper.create_user_buttons('CORRECT', 'WRONG')

	-- return info
	return {
		answer = ANSWER,
		winner = 1,
		title = '3rd Round 3rd step Final',
		subtitle = 'KiRa-KiRa Sensation',
		quizid = 303,
		org_user = ash_helper.all_zero_user
	}
end

function on_user_button(index, id)
	ash.save()

	local user, data, info = ash.get_user(index), {}, {}

	if id == 1 then	-- correct
		data.correct = user.correct + 1
		data.score = user.score + 1

		if suc_index == index then	-- ˜A“š
			dir_count = (dir_count + 1) % 2
			table.insert(info, 30301 + dir_count)
		else
			suc_index = index
		end

		if data.score >= SCORE_LIMIT then	-- clear
			table.insert(info, ash_helper.has_won_ai)
		end
	elseif id == 2 then	-- wrong
		data.wrong = user.wrong + 1

		local target = ash.get_user((index + dir_count) % ANSWER + 1)
		if target.score + 1 >= SCORE_LIMIT then -- clear
			ash.set_user(target.index, { score = target.score + 1 }, {ash_helper.has_won_ai, 30303})
		else
			ash.set_user(target.index, { score = target.score + 1 })
		end

		if suc_index == index then
			suc_index = 0
		end
	end

	ash.set_user(index, data, info)
end

