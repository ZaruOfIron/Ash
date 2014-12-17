require('ash_helper')

ANSWER = 10
WINNER = 4
add_score = 0
longest_word_counts = {}

function initialize()
	-- create user buttons
	ash_helper.create_user_buttons('CORRECT', 'WRONG')

	-- create system buttons	
	ash_helper.create_system_buttons(
		'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'CLEAR', 'FINISH'
	)

	for i = 1, ANSWER do
		table.insert(longest_word_counts, 0)
	end

	-- return info
	return {
		answer = ANSWER,
		winner = WINNER,
		title = '3rd Round 2nd step',
		subtitle = 'コースβ 凛は口先番長なお前らも大好きだにゃ～',
		quizid = 203,
		org_user = ash_helper.all_zero_user
	}
end

function on_system_button(id)
	if id >= 1 and id <= 10 then	-- 数値入力
		add_score = add_score * 10 + id % 10
	elseif id == 11 then	-- clear
		add_score = 0
	elseif id == 12 then	-- finish
		local users = ash_helper.get_all_users(ANSWER)
		table.sort(users,
			function(a, b)
				if a.score ~= b.score then return a.score > b.score end
				if longest_word_counts[a.index] ~= longest_word_counts[b.index] then
					return longest_word_counts[a.index] > longest_word_counts[b.index]
				end
				if a.wrong ~= b.wrong then return a.wrong < b.wrong end
				return a.index < b.index
			end)
		for i = 1, WINNER do
			ash.set_user(users[i].index, {}, {1})
		end
	end
end

function on_user_button(index, id)
	local user, data = ash.get_user(index), {}

	if id == 1 then	-- correct
		data.correct = user.correct + 1
		data.score = user.score + add_score
		if longest_word_counts[index] < add_score then longest_word_counts[index] = add_score end
	elseif id == 2 then	-- wrong
		data.wrong = user.wrong + 1
		data.score = user.score - add_score
	end

	ash.set_user(index, data)
	add_score = 0
end

