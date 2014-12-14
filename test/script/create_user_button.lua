function initialize()
	ash.config.create_user_button(1, "CORRECT")

	return { answer = 12, winner = 5, title = '1st stage', subtitle = 'ÇTÅõÇQÅ~', quizid = 101, org_user = {correct = 1, wrong = 2, score = 3} }
end

function on_command(index, id)
	print(string.format('idx:%d, id:%d', index, id))
end
