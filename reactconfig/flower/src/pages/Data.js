import React from 'react'
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend, ResponsiveContainer } from 'recharts';
import '../App.css';

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
        const url = window.url_prefix + '/config/data/data.json';
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
    renderchart ()
    {
        console.log ("render chart");
        return (
            
            <LineChart
              width={1024}
              height={300}
              data={this.state.data.mesures}
              margin={{
                top: 5,
                right: 30,
                left: 20,
                bottom: 5,
              }}
            >
              <CartesianGrid strokeDasharray="3 3" />
              <XAxis dataKey="id" />
              <YAxis />
              <Tooltip />
              <Legend />
              
              <Line type="monotone" dataKey="value" stroke="#82ca9d" />
            </LineChart>
          
        )
    }
    render() {
        const items = this.state.data;
        const msg = this.state.fetchmsg;
        let datajsx = "";
        let graphjsx = "";

        console.log ("before data analyze", items);
        if (items.mesures && items.mesures.length > 0)
        {
            console.log ("data analyze");
            datajsx = items.mesures.map((item) =><p> {item.id} / {item.value}</p>);
            graphjsx = this.renderchart ();
        }
        return (
                <div class="graphbox">
                    <p>{this.state.name}</p>
                
                    <div >
                        {
                            graphjsx
                        }
                    </div>
               
                
                    <p>{msg}</p>
                </div>
            
          );
    }
  };
  
  