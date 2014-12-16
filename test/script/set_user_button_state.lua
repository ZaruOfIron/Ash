function initialize()
	ash.config.create_user_button(1, 'PUSH')

	return { answer = 12, winner = 5, title = '1st stage', subtitle = '‚T›‚Q~', quizid = 101, org_user = {correct = 1, wrong = 2, score = 3} }
end

function on_command(index, id)
	ash.set_user_button_state(index, id, false)
end
