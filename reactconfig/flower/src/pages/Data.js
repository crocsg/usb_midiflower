import React from 'react'

export default class Data extends React.Component {
    
    constructor(props) {
        super(props);
        this.state = {
            name:"a very strange name",
            fetchmsg:"",
            data:[],

    
        };
    }
    componentDidMount ()
    {
        this.loaddata();
    }
    loaddata ()
    {
        console.log ("fetching data");
        const url = '/config/data/data.json';
        fetch (url)
            .then (res => res.json())
            .catch (() => {
                this.setState ({fetchmsg:"error loading json"})
                })
            .then (
                (result) => {
                    if (result)
                    {
                        console.log (result);
                        this.setState ({data:result})
                    }

                }
            )    
    }
    render() {
        const items = this.state.data;
        const msg = this.state.fetchmsg;
        let datajsx = "";
        console.log ("before data analyze", items);
        if (items.mesures && items.mesures.length > 0)
        {
            console.log ("data analyze");
            datajsx = items.mesures.map((item) =><p> {item.id} / {item.value}</p>);
        }
        return (
                <div>
                <p>{this.state.name}</p>
                <p><strong>Data page</strong></p>
                <p>Lorem ipsum ....</p>
                <p>Autre parapgrame</p>
                <p>data fetched</p>
                {
                    datajsx
                }
                <p>{msg}</p>
                </div>
            
          );
    }
  };
  
  