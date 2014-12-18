require('ash_helper')

ANSWER = 9
WINNER = 1
CHALLENGE_CORRECT = 5
WRONG_LIMIT = 3

now_challenging = false
seed_indexes = {}

function export_save_data()
	return ash_helper.serialize(ash_helper.create_twin_table('now_challenging', 'seed_indexes'))
end

function import_save_data(str)
	local data = ash_helper.deserialize(str)
	now_challenging = data.now_challenging
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
	ash.save()

	local user, data, info = ash.get_user(index), {}, {}

	if id == 1 then	-- correct
		data.correct = user.correct + 1

		if now_challenging then
			table.insert(info, 1)
			now_challenging = false
		else
			if data.correct >= CHALLENGE_CORRECT then
				if ash_helper.search_array(seed_indexes, index) then
					table.insert(info, 1)
				else
					now_challenging = true
					table.insert(info, 10301)
				end
			end
		end
	elseif id == 2 then	-- wrong
		if now_challenging then
			data.correct = CHALLENGE_CORRECT - 2
			now_challenging = false
			table.insert(seed_indexes, index)
		else
			data.wrong = user.wrong + 1

			if data.wrong >= WRONG_LIMIT then
				table.insert(info, 2)
			end
		end
	end

	ash.set_user(index, data, info)
end

