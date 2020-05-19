local this = {}

this.RequestState = {
    Init = 1,
    Processing = 2,
    Finished = 3,
}


this.ReturnState = {
    Processing = 1,
    Finished = 2,
    Break = 3,
}

return this