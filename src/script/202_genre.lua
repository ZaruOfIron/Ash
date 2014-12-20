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
	ash_helper.create_user_buttons('CORRECT', 'WRONG')

	-- create system buttons	
	ash_helper.create_system_buttons('FINISH')

	-- return info
	return {
		answer = ANSWER,
		winner = WINNER,
		title = '3rd Round 2nd step First set',
		subtitle = 'ƒR[ƒXƒ¿ ‚â‚è‚½‚¢‚±‚Æ‚Í',
		quizid = 202,
		org_user = ash_helper.all_zero_user
	}
end

function on_system_button(id)
	ash.save()

	if id == 1 then	-- finish
		local users = ash_helper.get_all_users(ANSWER)
		-- socre => correct => index
		table.sort(users,
			function(a, b)
				print(string.format('%d, %d', a.score, b.score))
				print(string.format('  %d', a.score > b.score and 1 or 0))
				if a.score ~= b.score then return a.score > b.score end
				print('DEBUG')
				if a.correct ~= b.correct then return a.correct > b.correct end
				return a.index < b.index
			end)
		for i = 1, WINNER do
			ash_helper.send_win_without_order(users[i].index)
		end
	end
end

function on_user_button(index, id)
	ash.save()

	local user, data = ash.get_user(index), {}

	if id == 1 then	-- correct
		data.correct = user.correct + 1
		data.score = user.score + 1
	elseif id == 2 then	-- wrong
		data.wrong = user.wrong + 1
		data.score = user.score - 1
	end

	ash.set_user(index, data)
end

