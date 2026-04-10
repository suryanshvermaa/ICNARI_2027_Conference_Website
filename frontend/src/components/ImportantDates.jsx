const dates = [
  {
    title: "Paper Submission Start Date",
    date: "June 16",
    description: "Start date to submit research papers for review.",
  },
    {
      title: "Paper Submission Deadline",
      date: "December 18 (extended)",
      description: "Final date to submit research papers for review.",
    },
    {
      title: "Notification of Acceptance",
      date: "25 January, 2027",
      description: "Authors will receive acceptance or rejection notifications.",
    },
    {
      title: "Early Bird Registration",
      date: "15 February, 2027",
      description: "Deadline for early bird registration for the conference.",
    },
    {
      title: "Camera-Ready Submission",
      date: "20 February, 2027",
      description: "Final versions of accepted papers must be submitted.",
    },
    {
      title: "Last Date of Registration",
      date: "25 February, 2027",
      description: "Final date for all participants to register for the conference.",
    },
    {
      title: "Conference Start Date",
      date: "March 6, 2027",
      description: "The official start of the International Conference.",
    },
    {
      title: "Conference End Date",
      date: "March 8, 2027",
      description: "The final day of the conference.",
    },
  ];

const EventTimeline = () => {  
  return (
    <div className="max-w-7xl mx-auto py-12 px-6">
      <h2 className="text-3xl font-bold text-center text-zinc-900 dark:text-slate-50 mb-8">
        Important Dates
      </h2>
      <div className="site-card p-6 ring-1 ring-inset ring-indigo-200 dark:ring-indigo-500/25">
        <table className="min-w-full">
          <thead>
            <tr>
              <th className="text-left p-2 border-b-2 border-indigo-300 dark:border-indigo-400/30 text-zinc-900 dark:text-slate-50">Event</th>
              <th className="text-left p-2 border-b-2 border-indigo-300 dark:border-indigo-400/30 text-zinc-900 dark:text-slate-50">Date</th>
              <th className="text-left p-2 border-b-2 border-indigo-300 dark:border-indigo-400/30 text-zinc-900 dark:text-slate-50">Description</th>
            </tr>
          </thead>
          <tbody>
            {dates.map((event, index) => (
              <tr key={index} className="hover:bg-indigo-50 transition-colors dark:hover:bg-slate-800/60">
                <td className={`p-2 border-b border-zinc-200 dark:border-slate-700/60 ${index >= 2 ? 'text-indigo-700 dark:text-indigo-200 font-bold' : 'text-zinc-800 dark:text-slate-100 font-medium'}`}>{event.title}</td>
                <td className={`p-2 border-b border-zinc-200 dark:border-slate-700/60 ${index >= 2 ? 'text-indigo-700 dark:text-indigo-200 font-bold' : 'text-zinc-800 dark:text-slate-100 font-medium'}`}>{event.date}</td>
                <td className={`p-2 border-b border-zinc-200 dark:border-slate-700/60 ${index >= 2 ? 'text-zinc-800 dark:text-slate-100 font-semibold' : 'text-zinc-600 dark:text-slate-300'}`}>{event.description}</td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>
    </div>
  );
};

export default EventTimeline;
