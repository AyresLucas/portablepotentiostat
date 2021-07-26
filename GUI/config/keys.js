dbPassword = 'mongodb+srv://lucasayres:'+ encodeURIComponent('12101994') + '@cluster0.arbq7.mongodb.net/ulab?retryWrites=true'

module.exports = {
    mongoURI: dbPassword
};


//mongodb+srv://lucasayres:<password>@cluster0.arbq7.mongodb.net/<dbname>?retryWrites=true&w=majority