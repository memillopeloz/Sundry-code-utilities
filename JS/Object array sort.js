// This allows you to create reusable sort functions, and sort by any field.

var sort_by = function(field, reverse, primer){

   var key = primer ? 
       function(x) {return primer(x[field])} : 
       function(x) {return x[field]};

   reverse = [-1, 1][+!!reverse];

   return function (a, b) {
       return a = key(a), b = key(b), reverse * ((a > b) - (b > a));
     } 
}

// Now you can sort by any field at will...

var homes = [{

   "h_id": "3",
   "city": "Dallas",
   "state": "TX",
   "zip": "75201",
   "price": "162500"

}, {

   "h_id": "4",
   "city": "Bevery Hills",
   "state": "CA",
   "zip": "90210",
   "price": "319250"

}, {

   "h_id": "5",
   "city": "New York",
   "state": "NY",
   "zip": "00010",
   "price": "962500"

}];

// Sort by price high to low
homes.sort(sort_by('price', true, parseInt));

// Sort by city, case-insensitive, A-Z
homes.sort(sort_by('city', false, function(a){return a.toUpperCase()}));
