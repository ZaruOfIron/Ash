require('ash_helper')

ANSWER = 9
WINNER = 1
CHALLENGE_CORRECT = 5
WRONG_LIMIT = 3

challenging_index = 0
seed_indexes = {}

function export_save_data()
	return ash_helper.serialize(ash_helper.create_twin_table('challenging_index', 'seed_indexes'))
end

function import_save_data(str)
	local data = ash_helper.deserialize(str)
	challenging_index = data.challenging_index
	seed_indexes = data.seed_indexes
end

function initialize()
	-- create user buttons
	ash_helper.create_user_buttons('CORRECT', 'WRONG')

	-- return info
	return {
		answer = ANSWER,
		winner = WINNER,
		title = '3rd Round 2nd step Final set',
		subtitle = '通過ぁ？ミトメラレナイワァ!!',
		quizid = 103,
		org_user = ash_helper.all_zero_user
	}
end

function on_user_button(index, id)
	-- 封筒挑戦中にはその答者以外は操作できない
	if challenging_index ~= 0 and challenging_index ~= index then
		return
	end

	ash.save()

	local user, data, info = ash.get_user(index), {}, {}

	if id == 1 then	-- correct
		data.correct = user.correct + 1

		if challenging_index ~= 0 then
			table.insert(info, 1)
			challenging_index = 0
		else
			if data.correct >= CHALLENGE_CORRECT then
				if ash_helper.search_array(seed_indexes, index) then
					table.insert(info, 1)
				else
					challenging_index = index
					table.insert(info, 10301)
				end
			end
		end
	elseif id == 2 then	-- wrong
		if challenging_index ~= 0 then
			data.correct = CHALLENGE_CORRECT - 2
			table.insert(seed_indexes, index)
			challenging_index = 0
		else
			data.wrong = user.wrong + 1

			if data.wrong >= WRONG_LIMIT then
				table.insert(info, 2)
			end
		end
	end

	ash.set_user(index, data, info)
end

