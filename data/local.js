
    
(function worker() {
    $.getJSON({
        url:"rt", 
        success: function(data) {
            $.each(data, function(k, v){
                $('.'+k).text(v);
            })},
        complete: function() {
        // Schedule the next request when the current one's complete
            setTimeout(worker, 2000);
        }
    });
  })();